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

#ifndef request_state_manager_hpp
#define request_state_manager_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <mutex>

#include "common.h"

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
    void set_request_state( const std::string request_id, std::shared_ptr<boost::property_tree::ptree> current_state )
    {
        std::lock_guard<std::mutex> lock(_state_map_mutex);
        
        _state_map[request_id] = current_state;
    }
    
    std::shared_ptr<boost::property_tree::ptree> get_request_state( const std::string request_id )
    {
        std::lock_guard<std::mutex> lock(_state_map_mutex);
        
        if ( _state_map.find(request_id) == _state_map.end() )
        {
            auto request_not_found = std::make_shared<boost::property_tree::ptree>();
            request_not_found->put("request_id", request_id);
            request_not_found->put("status", static_cast<int>(request_state_manager::request_state::not_found));
            
            return request_not_found;
        }
        
        return _state_map[ request_id ];
    }
    
private:
    std::map<std::string, std::shared_ptr<boost::property_tree::ptree>> _state_map;
    std::mutex _state_map_mutex;
    
};

#endif /* request_state_manager_hpp */
