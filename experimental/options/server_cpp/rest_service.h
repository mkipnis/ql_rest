// Copyright (C) Mike Kipnis

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "redis_cache.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using namespace boost::posix_time;


namespace rest_service {

std::string timestamp_now() {

    static std::locale loc{
        std::locale::classic(),
        new time_facet("%Y-%m-%d %H:%M:%S.%f")
    };

    std::ostringstream oss;
    oss.imbue(loc);
    oss << microsec_clock::local_time();

    return oss.str();
};

struct PricingRequest {

    std::string token;
    json::value request;
};

using PricingRequestPtr = PricingRequest*;

using PricingRequestQueue =
    boost::lockfree::queue<PricingRequestPtr>;

using PricingRequestQueuePtr =
    std::shared_ptr<PricingRequestQueue>;


//------------------------------------------------------------------------------

// Return a response for the given request.
//
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator.
template <class Body, class Allocator>
http::message_generator
handle_request(
    PricingRequestQueuePtr& queue,
    http::request<Body, http::basic_fields<Allocator>>&& req)
{
    namespace http = boost::beast::http;
    namespace json = boost::json;

    json::object response;
    http::status status = http::status::ok;

    try
    {
        const std::string target = std::string(req.target());

        std::cout << "Method : " << req.method_string() << std::endl;
        std::cout << "Target : " << target << std::endl;

        
        auto& redis = redis_client::get_thread_redis();
        
        // ============================================================
        // POST /price
        // ============================================================
        if (req.method() == http::verb::post &&
            target == "/price")
        {
            auto body = json::parse(req.body()).as_object();

            boost::uuids::uuid uuid =
                boost::uuids::random_generator()();

            std::string token =
                boost::uuids::to_string(uuid);

            auto pricing_request = new PricingRequest{
                token,
                body
            };

            // enqueue pricing_request into worker queue
            queue->push(pricing_request);

            json::object request_state;
            request_state["token"] = token;
            request_state["status"] = "queued";
            request_state["timestamp"] =
                rest_service::timestamp_now();

            redis.set(
                token,
                json::serialize(request_state));

            response = request_state;
        }

        // ============================================================
        // GET /result/<token>
        // ============================================================
        else if (req.method() == http::verb::get)
        {
            const std::string prefix = "/result/";

            if (target.find(prefix) == 0)
            {
                std::string token =
                    target.substr(prefix.size());

                std::cout << "Token : "
                          << token
                          << std::endl;

                auto value = redis.get(token);

                if (value)
                {
                    response =
                        json::parse(*value).as_object();
                }
                else
                {
                    status = http::status::not_found;

                    response["error"] =
                        "token not found";

                    response["token"] = token;
                }
            }
            else
            {
                status = http::status::not_found;

                response["error"] =
                    "unknown endpoint";
            }
        }

        // ============================================================
        // Unsupported Method
        // ============================================================
        else
        {
            status = http::status::bad_request;

            response["error"] =
                "unsupported request";
        }
    }
    catch (const std::exception& e)
    {
        status = http::status::internal_server_error;

        response["error"] = e.what();
    }

    std::string body = json::serialize(response);

    http::response<http::string_body> res{ status, req.version() };

    res.set( http::field::server, BOOST_BEAST_VERSION_STRING );
    res.set( http::field::content_type, "application/json" );
    res.set( http::field::access_control_allow_origin, "*" );
    res.set( http::field::access_control_allow_headers, "*" );
    res.keep_alive(req.keep_alive());

    res.body() = std::move(body);

    res.prepare_payload();

    return res;
}

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    PricingRequestQueuePtr queue_;
    http::request<http::string_body> req_;
    
public:
    // Take ownership of the stream
    session(
            tcp::socket&& socket,
            PricingRequestQueuePtr queue)
    : stream_(std::move(socket))
    , queue_(queue)
    {
    }
    
    // Start the asynchronous operation
    void
    run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                                                &session::do_read,
                                                shared_from_this()));
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
        send_response(
                      handle_request(queue_, std::move(req_)));
    }
    
    void
    send_response(http::message_generator&& msg)
    {
        bool keep_alive = msg.keep_alive();
        
        // Write the response
        beast::async_write(
                           stream_,
                           std::move(msg),
                           beast::bind_front_handler(
                                                     &session::on_write, shared_from_this(), keep_alive));
    }
    
    void
    on_write(
             bool keep_alive,
             beast::error_code ec,
             std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        
        if(ec)
            return fail(ec, "write");
        
        if(! keep_alive)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }
        
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
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    rest_service::PricingRequestQueuePtr queue_;
    
public:
    listener(
             net::io_context& ioc,
             tcp::endpoint endpoint,
             rest_service::PricingRequestQueuePtr queue)
    : ioc_(ioc)
    , acceptor_(net::make_strand(ioc))
    {
        queue_ = queue;
        
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
            return; // To avoid infinite loop
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                                      std::move(socket), queue_)->run();
        }
        
        // Accept another connection
        do_accept();
    }
};

};
