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

#include "swap_pricer_listener.hpp"
#include <boost/beast/core.hpp>
#include <boost/asio/dispatch.hpp>
#include <iostream>
#include "swap_pricer_session.hpp"

swap_pricer_listener::swap_pricer_listener( std::shared_ptr<pricing_thread>& pricing,
                                           boost::asio::io_context& ioc,
                                           boost::asio::ip::tcp::tcp::endpoint endpoint)
    : _acceptor(ioc) ,_socket(ioc) ,_pricing_thread(pricing)
{
    boost::system::error_code ec;

    // Open the acceptor
    _acceptor.open(endpoint.protocol(), ec);
    if(ec)
    {
        std::cerr << "open" << ec.message() << std::endl;
        throw std::runtime_error(ec.message());
    }

    // Allow address reuse
    _acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    if(ec)
    {
        std::cerr << "set_option" << ec.message() << std::endl;
        throw std::runtime_error(ec.message());
    }

    // Bind to the server address
    _acceptor.bind(endpoint, ec);
    if(ec)
    {
        std::cerr << "bind" << ec.message() << std::endl;
        throw std::runtime_error(ec.message());
    }

    // Start listening for connections
    _acceptor.listen( boost::asio::socket_base::max_listen_connections, ec );
    if(ec)
    {
        std::cerr << " listen" << ec.message() << std::endl;
        throw std::runtime_error(ec.message());
    }
}

void swap_pricer_listener::on_accept(boost::system::error_code ec)
{
    if(ec)
    {
        std::cerr << "accept" << ec.message() << std::endl;
        return;
    }
    else
    {
        // Create the session and run it
        std::make_shared<ql_rest_swap_pricer_session>(_pricing_thread, std::move(_socket))->run();
    }

    // Accept another connection
    do_accept();
}

void swap_pricer_listener::do_accept()
{
    _acceptor.async_accept( _socket,
                           boost::beast::bind_front_handler( &swap_pricer_listener::on_accept,
                                                            shared_from_this()) );
}

void swap_pricer_listener::run()
{
    if(!_acceptor.is_open())
        return;
    do_accept();
}
