/*
    Copyright (C) 2022 Mike Kipnis

    This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


#ifndef basic_microservice_hpp
#define basic_microservice_hpp

#include <stdio.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

#include <boost/json.hpp>

#include <ql/quantlib.hpp>
#include <Addins/Cpp/addincpp.hpp>
#include <oh/ohdefines.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>
#include <oh/ohdefines.hpp>
#include <qlo/qladdin.hpp>
#include <Addins/Cpp/addincpp.hpp>

#include <boost/lockfree/queue.hpp>

#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace ql_rest
{

using json_raw_ptr = boost::json::object*;
using json_ptr_pc_queue = boost::lockfree::queue< json_raw_ptr >;
using json_ptr_pc_queue_ptr = std::shared_ptr<json_ptr_pc_queue>;

using pricing_function = std::function<std::tuple<std::string, boost::json::object>( json_raw_ptr )>;

auto const print_json =
[](boost::json::object& pricer_request)
{
    std::cout << boost::json::serialize(pricer_request) << std::endl;
};


class request_state_manager
{
    
public:
    enum class request_state
    {
        not_found = 0,
        pending = 1,
        processed = 2
    };
    
public:
    void set_request_state( const std::string request_id, boost::json::object& current_state )
    {
        std::lock_guard<std::mutex> lock(_state_map_mutex);
        
        _state_map[request_id] = current_state;
    }
    
    boost::json::object get_request_state( const std::string request_id )
    {
        std::lock_guard<std::mutex> lock(_state_map_mutex);
        
        if ( _state_map.find(request_id) == _state_map.end() )
        {
            boost::json::object request_not_found;
            request_not_found["request_id"] = request_id;
            request_not_found["status"] = static_cast<int>(request_state_manager::request_state::not_found);
            
            return request_not_found;
        }
        
        return _state_map[ request_id ];
    }
    
private:
    std::map<std::string, boost::json::object> _state_map;
    std::mutex _state_map_mutex;
    
};


class pricing_thread
{
    std::atomic<bool> _done {};
    json_ptr_pc_queue_ptr _pricer_queue;
    request_state_manager _request_state_manager;
    
    pricing_function _pricing_function;
    
    std::thread _thread;
    
public:
    pricing_thread( pricing_function& function ) :  _pricing_function(function)
    {
        _pricer_queue = std::make_shared<json_ptr_pc_queue>(128);
        _thread = std::thread(&pricing_thread::run_pricer,this);
    };
    
    void run_pricer() {
           
        QuantLibAddinCpp::initializeAddin();
        
        while (!this->_done.load())
        {
            json_raw_ptr pricer_request;
         
            while (_pricer_queue->pop(pricer_request))
            {
                // print_json(*pricer_request);
                
                auto results = _pricing_function(pricer_request);
                    
                std::get<1>(results)["state"] = static_cast<int>(request_state_manager::request_state::processed);
                _request_state_manager.set_request_state( std::get<0>(results),  std::get<1>(results));
                
                delete pricer_request;
                ObjectHandler::Repository::instance().deleteAllObjects(true);
            };
            
        };
    };
        
    void stop() {
           _done.store(true);
    };
        
        
    void enqueue_request(std::string request_id, json_raw_ptr request_ptr) {
        
        _pricer_queue->push(request_ptr);
        boost::json::object pending_request_state;
        pending_request_state["request_id"] = request_id;
        pending_request_state["state"] = static_cast<int>(request_state_manager::request_state::pending);
        _request_state_manager.set_request_state(request_id, pending_request_state);
    }
    
    boost::json::object get_request_state( std::string request_id ){
        return _request_state_manager.get_request_state( request_id );
    }
};


// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<
    class Body, class Allocator,
    class Send>
void
handle_request(
    std::shared_ptr<pricing_thread> pricing_thread,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if( req.method() != http::verb::options &&
        req.method() != http::verb::post &&
        req.method() != http::verb::get)
            return send(bad_request("Unknown HTTP-method"));

    std::ostringstream oss;
    
    if(req.method() == http::verb::post || req.method() == http::verb::get )
    {
        std::string request_id;
        
        if ( req.target() == "/submit_request/")
        {
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            json_raw_ptr request = new boost::json::object(boost::json::parse(req.body()).as_object());

            request_id = boost::uuids::to_string(uuid);
            (*request)["request_id"] = request_id;

            pricing_thread->enqueue_request(request_id, request);

        } else if ( req.target() == "/check_request/" )
        {
            boost::json::object request = boost::json::parse(req.body()).as_object();
            request_id = boost::json::value_to<std::string>(request["request_id"]);

        }
        
        auto request_state = pricing_thread->get_request_state(request_id);
        
        oss << boost::json::serialize(request_state);
    }
    
    boost::beast::http::string_body::value_type string_body(oss.str());
    
    boost::beast::http::response<boost::beast::http::string_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(string_body)),
        std::make_tuple(boost::beast::http::status::ok, req.version()) };
              
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "application/json");
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::access_control_allow_headers, "*");
    res.keep_alive(req.keep_alive());
        
    return send(std::move(res));
}


// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    struct send_lambda
    {
        session& self_;

        explicit
        send_lambda(session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
        operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<
                http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.stream_,
                *sp,
                beast::bind_front_handler(
                    &session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<pricing_thread> pricing_thread_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;

public:
    // Take ownership of the stream
    session(
        tcp::socket&& socket,
        std::shared_ptr<pricing_thread>& pricing_thread)
        : stream_(std::move(socket))
        , pricing_thread_(pricing_thread)
        , lambda_(*this)
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        do_read();
    }

    void
    do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return do_close();

        if(ec)
            return fail(ec, "read");

        // Send the response
        handle_request(pricing_thread_, std::move(req_), lambda_);
    }

    void
    on_write(
        bool close,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        if(close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // We're done with the response so delete it
        res_ = nullptr;

        // Read another request
        do_read();
    }

    void
    do_close()
    {
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
    
    void
    fail(beast::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<pricing_thread> pricing_thread_;

public:
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint, pricing_function&& function)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc))
        , pricing_thread_(std::make_shared<pricing_thread>(function))
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {

        do_accept();
    }
    
    void
    fail(beast::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }

private:
    void
    do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket),
                pricing_thread_)->run();
        }

        // Accept another connection
        do_accept();
    }
};
    
}

#endif /* basic_microservice_hpp */
