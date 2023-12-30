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
#include <qlrest/basic_microservice.hpp>

#include <iostream>
#include <tuple>

#include <qlrest/volatilities_reader.hpp>
#include <qlrest/payoffs_reader.hpp>
#include <qlrest/processes_reader.hpp>
#include <qlrest/exercise_reader.hpp>


#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>
#include <oh/ohdefines.hpp>
#include <qlo/qladdin.hpp>
#include <Addins/Cpp/addincpp.hpp>

#include "european_option_helper.h"
#include "american_option_helper.h"

#include <boost/json/src.hpp>

auto options_pricer_request_processor = [] (ql_rest::json_raw_ptr pricer_request)
{
    boost::json::object pricer_results;
    auto request_id = boost::json::value_to<std::string>(pricer_request->at("request_id"));
    
    try
     {
         pricer_results["request_id"] = request_id;
         
         std::cout << "Request In: " << *pricer_request << std::endl;
         
         // Let's adjust the business date to a valid valuation date
         auto business_date = ObjectHandler::Create<QuantLib::Calendar>() ( "TARGET" ).adjust(
                             QuantLib::DateParser::parseFormatted(
                                                                  boost::json::value_to<std::string>(pricer_request->at("business_date")),"%Y-%m-%d"),
                             ObjectHandler::Create<QuantLib::BusinessDayConvention>() ( ObjectHandler::property_t("Modified Following") ) );
         
         QuantLibAddinCpp::qlSettingsSetEvaluationDate( (long) business_date.serialNumber(), OH_NULL);
         QuantLib::Calendar settlement_calendar = QuantLib::TARGET();
         auto settlement_date = settlement_calendar.advance(business_date, 0, QuantLib::Days);
         
         auto exercise_type = boost::json::value_to<std::string>(pricer_request->at("exercise_type"));
         
        if ( exercise_type == "American" )
        {
            pricer_results = american_option_helper::options_pricer(pricer_request, settlement_date);
        } else if ( exercise_type == "European" )
        {
            pricer_results = european_option_helper::options_pricer(pricer_request, settlement_date);
        }  else {
            pricer_results["error_code"] = -9;
            pricer_results["error"] = "Unsupported Exercise Type";
        }
            
                  
     } catch ( const std::exception& exp )
     {
         pricer_results["error_code"] = -2;
         pricer_results["error"] = exp.what();
         std::cout << "Exception : " << exp.what() << std::endl;
     }
    
    return std::make_tuple( request_id, pricer_results );
};


int main(int argc, const char * argv[]) {
    
    std::cout << "OptionsPricer" << std::endl;
    
    if ( argc != 3 )
    {
        std::cerr << "Usage: qlRestOptionsPricer <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto const address = boost::asio::ip::make_address( argv[1] );
    auto const port = static_cast<unsigned short>( std::atoi(argv[2]) );
    
    boost::asio::io_context ioc;
    
    
    try
    {
        std::make_shared<ql_rest::listener>( ioc, tcp::endpoint{address, port}, options_pricer_request_processor)->run();
        
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
