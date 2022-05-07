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

#include "bond_pricer_session.hpp"
#include <boost/asio/dispatch.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


bond_pricer_session::bond_pricer_session( std::shared_ptr<pricing_thread>& pricing, boost::asio::ip::tcp::tcp::socket socket )
    : _socket(std::move(socket))
   , _strand(_socket.get_executor())
    , _lambda(*this)
    ,_pricer( pricing )
{
}


template< class Body, class Allocator, class Send>
void bond_pricer_session::handle_request(  std::shared_ptr<pricing_thread> pricing, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req, Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if( req.method() != boost::beast::http::verb::get &&
        req.method() != boost::beast::http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Attempt to open the file
    boost::beast::error_code ec;
    boost::beast::http::file_body::value_type body;

    std::string request_id;
    
    if (req.method() == boost::beast::http::verb::get)
    {
        if ( req.target() == "/submit_request")
        {
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            std::cout << uuid << std::endl;
        
            json_raw_ptr request = new boost::property_tree::ptree();
            std::istringstream stream(req.body());
            boost::property_tree::read_json(stream, *request);
        
        
            request_id = boost::uuids::to_string(uuid);
            request->put("request_id", request_id);

            _pricer->enqueue_request(request_id, request);
        } else if ( req.target() == "/check_request" )
        {
            boost::property_tree::ptree request;
            std::istringstream stream(req.body());
            boost::property_tree::read_json(stream, request);
            
            request_id = request.get<std::string>("request_id");
        }
        
        auto request_state = _pricer->get_request_state(request_id);
        
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, *request_state);
        
        boost::beast::http::string_body::value_type string_body(oss.str());
        
        boost::beast::http::response<boost::beast::http::string_body> res{
            std::piecewise_construct,
            std::make_tuple(std::move(string_body)),
            std::make_tuple(boost::beast::http::status::ok, req.version()) };
        
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
     
        return send(std::move(res));
    }
}


// Start the asynchronous operation
void bond_pricer_session::run()
{
    do_read();
}

void bond_pricer_session::do_read()
{
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    _req = {};

    // Read a request
    boost::beast::http::async_read(_socket, _buffer, _req,
        boost::asio::bind_executor(
            _strand,
            std::bind(
                &bond_pricer_session::on_read,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2)));
}

void bond_pricer_session::on_read(
    boost::system::error_code ec,
    std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == boost::beast::http::error::end_of_stream)
        return do_close();

    if(ec)
    {
        std::cerr << "read" << ": " << ec.message() << "\n";
        return;
    }

    // Send the response
    handle_request(_pricer, std::move(_req), _lambda);
}

void bond_pricer_session::on_write(
    boost::system::error_code ec,
    std::size_t bytes_transferred,
    bool close)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
    {
        std::cerr << "Write" << ": " << ec.message() << "\n";
        return;
    }

    if(close)
    {
        return do_close();
    }

    _res = nullptr;

    do_read();
}

void bond_pricer_session::do_close()
{
    boost::system::error_code ec;
    _socket.shutdown(boost::asio::ip::tcp::tcp::socket::shutdown_send, ec);
}
