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

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/lockfree/queue.hpp>
#include <qlrest/qlrest_common.h>
#include <qlrest/basic_microservice.hpp>

#include <iostream>
#include <tuple>

#include <qlrest/index_reader.hpp>
#include <qlrest/ratehelpers_reader.hpp>
#include <qlrest/piecewiseyieldcurve_reader.hpp>
#include <qlrest/pricingengines_reader.hpp>
#include <qlrest/vanillaswap_reader.hpp>

#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>
#include <oh/ohdefines.hpp>
#include <qlo/qladdin.hpp>
#include <Addins/Cpp/addincpp.hpp>

#include <boost/json/src.hpp>

#include "swap_builder_helper.hpp"


auto swap_pricer_request_processor = [] (ql_rest::json_raw_ptr pricer_request)
{
    boost::json::object pricer_results;
    auto request_id = boost::json::value_to<std::string>(pricer_request->at("request_id"));
    
    try
     {
         auto operation = pricer_request->at("operation");
               
        // return indicatives
        pricer_results["request_id"] = request_id;
        pricer_results["operation"] = operation;
         
         // Let's adjust business date to a valid valuation date
         auto fixing_date = ObjectHandler::Create<QuantLib::Calendar>() ( "London Stock Exchange" ).adjust(
                             QuantLib::DateParser::parseFormatted(
                                                                  boost::json::value_to<std::string>( pricer_request->at("business_date") ),
                                                                  "%Y-%m-%d"),
                             ObjectHandler::Create<QuantLib::BusinessDayConvention>() ( ObjectHandler::property_t("Modified Following") ) );
         
         QuantLibAddinCpp::qlSettingsSetEvaluationDate( (long) fixing_date.serialNumber(), OH_NULL);
         
         //
         // Swap Curve Construction - Begin
         auto libor_index = ql_rest::index::qlLibor(pricer_request->at("index").as_object());
         
         auto swap_indexes = pricer_request->at("swap_indexes").as_array();
         std::for_each(swap_indexes.begin(), swap_indexes.end(), []( boost::json::value& json_val){
             ql_rest::index::qlLiborSwap(json_val.as_object());
         });
         
         auto swap_rate_helpers = pricer_request->at("swap_rate_helpers").as_array();
         std::for_each(swap_rate_helpers.begin(), swap_rate_helpers.end(), []( boost::json::value& json_val ){ ql_rest::ratehelpers::qlSwapRateHelper(json_val.as_object());
         });
 
         auto piecewise_yield_curve  = ql_rest::piecewiseyieldcurve::qlPiecewiseYieldCurve( pricer_request->at("piecewise_yield_curve").as_object() );
         
         auto index_with_curve = ql_rest::index::qlLibor(pricer_request->at("index_with_curve").as_object());
         
         auto discouning_engine = ql_rest::pricingengines::qlDiscountingSwapEngine(pricer_request->at("discounting_swap_engine").as_object());
         
         auto currency = boost::json::value_to<std::string>(pricer_request->at("index").as_object()["Currency"]);
         
         auto fixing_type = pricer_request->at("fixing_type");
         // Swap Curve Construction - End
         
         auto const get_libor_swap = [&]( const std::string& name, const std::string& tenor )
         {
             OH_GET_REFERENCE(swap_ptr,
                              QuantLibAddinCpp::qlLiborSwap(name, currency,
                                                            boost::json::value_to<std::string>(fixing_type),
                                                            tenor, piecewise_yield_curve,
                                                            piecewise_yield_curve, false, false, false),
                              QuantLibAddin::LiborSwap, QuantLib::SwapIndex);
             return swap_ptr->underlyingSwap(fixing_date);
         };

         if ( operation == "PRICE_CURVE")
         {
             auto swap_curve_point_request = pricer_request->at("swap_curve_point_request").as_array();
             boost::json::object curve_points;
                                             
             QuantLib::ClosestRounding closest_rate(3);
             
             for (auto it = swap_curve_point_request.begin(); it!=swap_curve_point_request.end(); ++it )
             {
                 auto tenor = boost::json::value_to<std::string>(*it);
                 auto swap_ptr = get_libor_swap("my_swap" + tenor, tenor);
                 curve_points[tenor] = closest_rate(swap_ptr->fairRate());
             }
             
             pricer_results["curve_points"] = curve_points;
         
         } else if ( operation == "PRICE_SWAP_AT_PAR")
         {
             auto tenor = boost::json::value_to<std::string>(pricer_request->at("tenor"));
             auto swap_ptr = get_libor_swap("my_swap" + tenor, tenor);
             auto swap_out = swap_designer::swap_to_json(swap_ptr);
             swap_out["pay_or_recieve"] = pricer_request->at("pay_or_recieve").as_string();
             swap_out["notional"] = pricer_request->at("notional").as_int64();
             swap_out["fixed_rate"] = swap_ptr->fairRate();
             swap_out["spread"] = 0.0;
             swap_out["engine"] = discouning_engine;
             swap_out["index"] = index_with_curve;
             auto test_swap = swap_designer::json_to_swap(swap_out, index_with_curve, discouning_engine );
             swap_out = swap_designer::swap_to_json(test_swap);
             pricer_results["my_swap"] = swap_out;
             
         } else if ( operation ==  "RE_PRICE_SWAP" )
         {
             auto swap_in = pricer_request->at("swap").as_object();
             auto swap_out = swap_designer::swap_to_json(swap_designer::json_to_swap(pricer_request->at("swap").as_object(), index_with_curve, discouning_engine ));
 
             pricer_results["my_swap"] = swap_out;
         }
     
         pricer_results["error_code"] = 0;
         
     } catch ( const std::exception& exp )
     {
         pricer_results["error_code"] = -2;
         pricer_results["error"] = exp.what();
         std::cout << "Exception : " << exp.what() << std::endl;
     }
    
    // std::cout << pricer_results << std::endl;
    
    return std::make_tuple( request_id, pricer_results );
};


int main(int argc, const char * argv[]) {
    
    std::cout << "qlRestSwapPricer" << std::endl;
    
    if ( argc != 3 )
    {
        std::cerr << "Usage: qlRestSwapPricer <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto const address = boost::asio::ip::make_address( argv[1] );
    auto const port = static_cast<unsigned short>( std::atoi(argv[2]) );
    
    boost::asio::io_context ioc;
    
    try
    {
        std::make_shared<ql_rest::listener>( ioc, tcp::endpoint{address, port}, swap_pricer_request_processor)->run();
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
