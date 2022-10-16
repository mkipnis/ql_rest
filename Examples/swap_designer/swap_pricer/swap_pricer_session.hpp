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

#ifndef bond_pricer_session_hpp
#define bond_pricer_session_hpp

#include <stdio.h>

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "common.h"
#include "pricing_thread.hpp"
#include <memory>


// Handles an HTTP server connection
class ql_rest_swap_pricer_session : public std::enable_shared_from_this<ql_rest_swap_pricer_session>
{
public:
    
    // Take ownership of the socket
    explicit ql_rest_swap_pricer_session( std::shared_ptr<pricing_thread>& pricing, boost::asio::ip::tcp::tcp::socket socket );
    
    template< class Body, class Allocator, class Send>
    void handle_request( std::shared_ptr<pricing_thread> pricing, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req, Send&& send);

    void run();
    void do_read();
    void on_read( boost::system::error_code ec, std::size_t bytes_transferred);
    void on_write( bool close, boost::system::error_code ec, std::size_t bytes_transferred);
    void do_close();
    
private:
    struct send_lambda
    {
        ql_rest_swap_pricer_session& self_;

        explicit
        send_lambda(ql_rest_swap_pricer_session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const
        {
            auto sp = std::make_shared<boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

            self_.res_ = sp;

            boost::beast::http::async_write( self_.stream_, *sp,
                    boost::beast::bind_front_handler(
                    &ql_rest_swap_pricer_session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

    boost::beast::flat_buffer buffer_;
    boost::beast::tcp_stream stream_;
    boost::beast::http::request<boost::beast::http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
    
    std::shared_ptr<pricing_thread> _pricer;
   };


#endif /* bond_pricer_session_hpp */
