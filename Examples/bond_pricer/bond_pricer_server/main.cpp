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

#include <qlrest/qlrest_common.h>
#include <qlrest/basic_microservice.hpp>
#include "curve_builder.hpp"
#include "bond_pricer.hpp"


auto bond_pricer_request_processor = [] (ql_rest::json_raw_ptr pricer_request)
{
    auto request_id = boost::json::value_to<std::string>(pricer_request->at("request_id"));
    // std::cout << "Request : " << request_id << std::endl;
    
    boost::json::object pricer_results;
    pricer_results["request_id"] = request_id;
    
    try
    {
        auto business_date_in = boost::json::value_to<std::string>(pricer_request->at("business_date"));
        auto business_date = ql_rest::from_iso_string(business_date_in);

        QuantLibAddinCpp::qlSettingsSetEvaluationDate( QuantLib::BigInteger(business_date), OH_NULL );
        
        auto curve = pricer_request->at("curve").as_object();
        auto bond_template = curve["bond_template"].as_object();
        auto curve_points = curve["curve_points"].as_object();
    
        curve_builder cb( request_id, curve_points, bond_template);
        
        pricer_results["yield_curve"] = cb.get_yield_curve();
        
        auto curve_object = cb.get_curve_object();
        auto bond_engine = cb.get_bond_engine();
        
        auto instruments = (*pricer_request)["instruments"].as_object();
            
        for (auto instrument = instruments.begin(); instrument != instruments.end(); ++instrument)
        {
            auto cusip = std::string(instrument->key());
            auto bond_object = instrument->value().as_object();
            
            auto bond_schedule = bond_object["ust_bond_schedule"].as_object();
            auto fixed_date_bond = bond_object["ust_fixed_rate_bond"].as_object();
         
            try
            {
                bond_pricer bp(bond_schedule, fixed_date_bond, bond_engine);
                
                boost::json::object bond_pricer_results;
                bp.populate_pricing_results(bond_pricer_results);
                                        
                pricer_results[cusip] = bond_pricer_results;
                
            } catch ( std::exception& exp )
            {
                std::cout << "Exception in bond setup Schedule : " << bond_schedule["ObjectId"].as_string()
                << " Fixed Rate Bond : " << fixed_date_bond["ObjectId"].as_string() << " : " << exp.what() << std::endl;
            }
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
    
    std::cout << "qlRestBondPricingServer" << std::endl;
    
    if ( argc != 3 )
    {
        std::cerr << "Usage: qlRestBondPricingServer <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto const address = boost::asio::ip::make_address( argv[1] );
    auto const port = static_cast<unsigned short>( std::atoi(argv[2]) );
    
    boost::asio::io_context ioc;
    
    try
    {
        std::make_shared<ql_rest::listener>( ioc, tcp::endpoint{address, port}, bond_pricer_request_processor)->run();
 
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
