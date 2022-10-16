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

#ifndef qlRestBondPricerListener_hpp
#define qlRestBondPricerListener_hpp

#include <stdio.h>

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>

#include "common.h"
#include "swap_pricer_session.hpp"

class swap_pricer_listener : public std::enable_shared_from_this<swap_pricer_listener>
{
    
    boost::asio::ip::tcp::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::tcp::socket _socket;
    std::shared_ptr<pricing_thread> _pricing_thread;
    
public:
    swap_pricer_listener( std::shared_ptr<pricing_thread>& pricing, boost::asio::io_context& ioc, boost::asio::ip::tcp::tcp::endpoint endpoint);

    void run();
    void do_accept();
    void on_accept(boost::system::error_code ec);
};

#endif
