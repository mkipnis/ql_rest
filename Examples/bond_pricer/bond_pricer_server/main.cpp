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

#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/lockfree/queue.hpp>

#include "bond_pricer_listener.hpp"
#include "pricing_thread.hpp"
#include "common.h"

int main(int argc, const char * argv[]) {
    
    std::cout << "qlRestBondPricingServer" << std::endl;
    
    if ( argc != 3 )
    {
        std::cerr << "Usage: qlRestBondPricingServer <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto const address = boost::asio::ip::make_address( argv[1] );
    auto const port = static_cast<unsigned short>( std::atoi(argv[2]) );

    std::shared_ptr<pricing_thread> pricing = std::make_shared<pricing_thread>();
    
    std::thread thread(&pricing_thread::run_pricer, pricing);
    
    boost::asio::io_context ioc;
    
    try
    {
        std::make_shared<bond_pricer_listener>(pricing, ioc,
                boost::asio::ip::tcp::endpoint{address, port})->run();
    } catch (std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM );
    signals.async_wait([&] ( boost::beast::error_code const&, int )
    {
        ioc.stop();
    	exit(0);
    });
    
    ioc.run();
    
    return EXIT_SUCCESS;
}
