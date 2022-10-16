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

#include <boost/asio/dispatch.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "bond_pricer_session.hpp"


bond_pricer_session::bond_pricer_session( std::shared_ptr<pricing_thread>& pricing, boost::asio::ip::tcp::tcp::socket socket )
    :
    stream_(std::move(socket)),
    lambda_(*this)
    ,_pricer( pricing )
{
}


template< class Body, class Allocator, class Send>
void bond_pricer_session::handle_request(  std::shared_ptr<pricing_thread> pricing, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req, Send&& send)
{
    if (req.method() == boost::beast::http::verb::post || req.method() == boost::beast::http::verb::options )
    {
        std::string request_id;

        if ( req.target() == "/submit_request/")
        {
            try
            {
                boost::uuids::uuid uuid = boost::uuids::random_generator()();
                std::cout << uuid << std::endl;
        
                json_weak_ptr request = new boost::property_tree::ptree();
                std::istringstream stream(req.body());
                boost::property_tree::read_json(stream, *request);

                request_id = boost::uuids::to_string(uuid);
                request->put("request_id", request_id);

                _pricer->enqueue_request(request_id, request);
            } catch ( ... )
            {
                std::cout << "Unknown Exception in submit request." << std::endl;
            }
            
        } else if ( req.target() == "/check_request/" )
        {
            try
            {
                boost::property_tree::ptree request;
                std::istringstream stream(req.body());
                boost::property_tree::read_json(stream, request);
            
                request_id = request.get<std::string>("request_id");

                std::cout << "Checking request : " << request.get<std::string>("request_id") << std::endl;
            } catch ( ... )
            {
                std::cout << "Unknown Exception in submit request." << std::endl;
            }
        }
        
        auto request_state = _pricer->get_request_state(request_id);
        
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, *request_state);
        
        //std::cout << "Processing post response : " << std::endl;
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

        
        // std::cout << "Sending out : " << res.body() << std::endl;
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
    req_ = {};

    // Read a request

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request
    boost::beast::http::async_read(stream_, buffer_, req_,
                                   boost::beast::bind_front_handler(
            &bond_pricer_session::on_read,
            shared_from_this()));
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
    handle_request(_pricer, std::move(req_), lambda_);
}

void bond_pricer_session::on_write(
    bool close,
    boost::system::error_code ec,
    std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
    {
        std::cerr << "Write" << ": " << ec.message() << "\n";
        return;
    }

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

void bond_pricer_session::do_close()
{
    // Send a TCP shutdown
    boost::beast::error_code ec;
    stream_.socket().shutdown(boost::asio::ip::tcp::tcp::socket::shutdown_send, ec);
}
