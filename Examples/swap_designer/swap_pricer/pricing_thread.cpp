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

#include "pricing_thread.hpp"

#include <chrono>
#include <thread>

#include "common.h"

#include <qlrest/schedule_reader.hpp>
#include <qlrest/bonds_reader.hpp>

#include <qlrest/index_reader.hpp>
#include <qlrest/ratehelpers_reader.hpp>
#include <qlrest/piecewiseyieldcurve_reader.hpp>
#include <qlrest/pricingengines_reader.hpp>
#include <qlrest/vanillaswap_reader.hpp>


#include <ql/time/date.hpp>

#include "swap_builder_helper.hpp"

pricing_thread::pricing_thread()
{
    QuantLibAddinCpp::initializeAddin();
    _pricer_queue = std::make_shared<json_ptr_pc_queue>(128);
};

void pricing_thread::run_pricer()
{
    std::cout << "Pricer thread" << std::endl;
    
    auto const print_json =
    [](boost::property_tree::ptree* pricer_request)
    {
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, *pricer_request);
        std::cout << oss.str() << std::endl;
    };
    
    while (!this->_done.load())
    {
        json_weak_ptr pricer_request;
     
        while (_pricer_queue->pop(pricer_request))
        {
            print_json(pricer_request);
            
            auto request_id = pricer_request->get<std::string>("request_id");
            auto operation = pricer_request->get<std::string>("operation");
            
            // return indicatives
            auto pricer_results = std::make_shared<boost::property_tree::ptree>();
            pricer_results->put("request_id", request_id);
            pricer_results->put("operation", operation);
            
            try
            {
                // Let's adjust business date to a valid valuation date
                auto fixing_date = ObjectHandler::Create<QuantLib::Calendar>() ( "London Stock Exchange" ).adjust(
                                    QuantLib::DateParser::parseFormatted( pricer_request->get<std::string>("business_date"),"%Y-%m-%d"),
                                    ObjectHandler::Create<QuantLib::BusinessDayConvention>() ( ObjectHandler::property_t("Modified Following") ) );
                
                QuantLibAddinCpp::qlSettingsSetEvaluationDate( (long) fixing_date.serialNumber(), OH_NULL);
                
                // 
                // Swap Curve Construction - Begin
                auto libor_index = ql_rest::index::qlLibor(pricer_request->get_child("index"));
                
                auto swap_indexes = pricer_request->get_child("swap_indexes");
                std::for_each(swap_indexes.begin(), swap_indexes.end(), []( ptree::value_type& pt){ ql_rest::index::qlLiborSwap(pt.second); });
                
                auto swap_rate_helpers = pricer_request->get_child("swap_rate_helpers");
                std::for_each(swap_rate_helpers.begin(), swap_rate_helpers.end(), []( ptree::value_type& pt ){ ql_rest::ratehelpers::qlSwapRateHelper(pt.second); });
        
                auto piecewise_yield_curve  = ql_rest::piecewiseyieldcurve::qlPiecewiseYieldCurve( pricer_request->get_child("piecewise_yield_curve") );
                
                auto index_with_curve = ql_rest::index::qlLibor(pricer_request->get_child("index_with_curve"));
                
                auto discouning_engine = ql_rest::pricingengines::qlDiscountingSwapEngine(pricer_request->get_child("discounting_swap_engine"));
                
                auto currency = pricer_request->get_child("index").get<std::string>("Currency");
                
                auto fixing_type = pricer_request->get<std::string>("fixing_type");
                // Swap Curve Construction - End
                
                auto const get_libor_swap = [&]( const std::string& name, const std::string& tenor )
                {
                    OH_GET_REFERENCE(swap_ptr,
                                     QuantLibAddinCpp::qlLiborSwap(name, currency, fixing_type, tenor, piecewise_yield_curve, piecewise_yield_curve, false, false, false),
                                     QuantLibAddin::LiborSwap, QuantLib::SwapIndex);
                    return swap_ptr->underlyingSwap(fixing_date);
                };

                if ( operation == "PRICE_CURVE")
                {
                    auto swap_curve_point_request = pricer_request->get_child("swap_curve_point_request");
                    boost::property_tree::ptree curve_points;
                                                    
                    QuantLib::ClosestRounding closest_rate(3);
                    
                    for (auto it = swap_curve_point_request.begin(); it!=swap_curve_point_request.end(); ++it )
                    {
                        auto tenor = it->second.data();
                        auto swap_ptr = get_libor_swap("my_swap" + tenor, tenor);
                        curve_points.put(tenor, closest_rate(swap_ptr->fairRate()));
                    }
                    
                    pricer_results->add_child("curve_points", curve_points);
                
                } else if ( operation == "PRICE_SWAP_AT_PAR")
                {
                    auto tenor = pricer_request->get<std::string>("tenor");
                    auto swap_ptr = get_libor_swap("my_swap" + tenor, tenor);
                    auto swap_out = swap_designer::swap_to_json(swap_ptr);
                    swap_out.put("pay_or_recieve", pricer_request->get<std::string>("pay_or_recieve"));
                    swap_out.put("notional", pricer_request->get<double>("notional"));
                    swap_out.put("fixed_rate", swap_ptr->fairRate());
                    swap_out.put("spread", 0.0);
                    swap_out.put("engine",  discouning_engine);
                    swap_out.put("index", index_with_curve);
                    auto test_swap = swap_designer::json_to_swap(swap_out, index_with_curve, discouning_engine );
                    swap_out = swap_designer::swap_to_json(test_swap);
                    pricer_results->add_child("my_swap", swap_out);
                    
                } else if ( operation ==  "RE_PRICE_SWAP" )
                {
                    auto swap_in = pricer_request->get_child("swap");
                    auto swap_in_with_curve = swap_designer::json_to_swap(swap_in, index_with_curve, discouning_engine );
                    auto swap_out = swap_designer::swap_to_json(swap_designer::json_to_swap(pricer_request->get_child("swap"), index_with_curve, discouning_engine ));
        
                    pricer_results->add_child("my_swap", swap_out);
                }
            
                std::ostringstream oss;
                boost::property_tree::json_parser::write_json(oss, *pricer_results);
                pricer_results->put("error_code", 0);
                
            } catch (const boost::property_tree::ptree_error& e )
            {
                std::cout << "Exception : " << e.what() << std::endl;
                pricer_results->put("error_code", -1);
                pricer_results->put("error", e.what());
                
            } catch ( const std::exception& exp )
            {
                pricer_results->put("error_code", -2);
                pricer_results->put("error", exp.what());
                std::cout << "Exception : " << exp.what() << std::endl;
            }
            
            pricer_results->put("state", static_cast<int>(request_state_manager::request_state::processed));
            _request_state_manager.set_request_state(request_id, pricer_results);
            ObjectHandler::Repository::instance().deleteAllObjects(true);
            
            delete pricer_request;
            
        }
    }
};

void pricing_thread::stop()
{
   _done.store(true);
}

void pricing_thread::enqueue_request(std::string request_id, json_weak_ptr request_ptr)
{
    _pricer_queue->push(request_ptr);

    auto pending_request_state = std::make_shared<boost::property_tree::ptree>();
    pending_request_state->put("request_id", request_id);
    pending_request_state->put("state", static_cast<int>(request_state_manager::request_state::pending));
    _request_state_manager.set_request_state(request_id, pending_request_state);
}

std::shared_ptr<boost::property_tree::ptree> pricing_thread::get_request_state( std::string request_id )
{
    return _request_state_manager.get_request_state( request_id );
}
