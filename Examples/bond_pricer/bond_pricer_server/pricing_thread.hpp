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


#ifndef pricing_thread_hpp
#define pricing_thread_hpp

#include <stdio.h>
#include <atomic>

#include "common.h"
#include "request_state_manager.hpp"


class pricing_thread
{
    std::atomic<bool> _done {};
    
    json_ptr_pc_queue_ptr _pricer_queue;
    
    request_state_manager _request_state_manager;
    
public:
    pricing_thread();
    
    void run_pricer();
    
    void stop();
    
    void enqueue_request(std::string request_id, json_raw_ptr);
    
    std::shared_ptr<boost::property_tree::ptree> get_request_state( std::string request_id );
};

#endif /* pricing_thread_hpp */
