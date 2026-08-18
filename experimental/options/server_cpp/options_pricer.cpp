// Copyright (c) Mike Kipnis

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lockfree/queue.hpp>


#include <hiredis/hiredis.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "redis_cache.h"
#include "rest_service.h"
#include "pricer_helper.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
namespace asio = boost::asio;

using tcp = net::ip::tcp;


using ThreadPoolPtr =
    std::shared_ptr<boost::asio::thread_pool>;


// ============================================================
// Dispatcher Worker
// ============================================================

void dispatcher( ThreadPoolPtr pool, rest_service::PricingRequestQueuePtr queue)
{
    
    while (true) {

        rest_service::PricingRequestPtr item;

        json::object status;
        
        if (queue->pop(item))
        {
            
            asio::post(*pool, [item]() {
                
                auto& redis = redis_client::get_thread_redis();

                try {                    
                
                    json::object status;
                
                    status["token"] = item->token;
                    status["status"] = "running";
                    status["timestamp"] = rest_service::timestamp_now();
                    
                    std::string status_txt = json::serialize(status);
                
                    redis.set(item->token, json::serialize(status));
                
                    auto start = std::chrono::steady_clock::now();
                    
                    auto result = options_pricer::process_price_request(item->request);
                    
                    auto latency_us = std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::steady_clock::now() - start).count();
                    
                    json::object completed;
                
                    completed["token"] = item->token;
                    completed["pricing_latency"] = latency_us;
                    completed["status"] = "completed";
                    completed["results"] = result;
                    completed["timestamp"] = rest_service::timestamp_now();
                
                    redis.set(item->token,json::serialize(completed));
                
                } catch (const std::exception& e) {
                
                    json::object failed;
                
                    failed["token"] = item->token;
                    failed["status"] = "failed";
                    failed["error"] = e.what();
                    failed["timestamp"] = rest_service::timestamp_now();
                
                    redis.set(item->token,json::serialize(failed));
                }
            
                delete item;
            
            });
            
        } else {
            std::this_thread::yield();
        }
    }
}



int main() {

    try {

        auto pool = std::make_shared<boost::asio::thread_pool>( std::thread::hardware_concurrency() );

        
        auto queue = std::make_shared<rest_service::PricingRequestQueue>(1024);
        
        std::thread dispatch_thread(
             [&]() {
                 dispatcher(pool, queue);
             }
         );

        Date today = Date::todaysDate();
    
        Settings::instance().evaluationDate() = today;
        
        net::io_context ioc{1};

        auto server = std::make_shared<rest_service::listener>( ioc, tcp::endpoint( tcp::v4(), 8000), queue );

        server->run();

        std::cout << "Server listening on port 8000" << std::endl;

        ioc.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    }

    return 0;
}
