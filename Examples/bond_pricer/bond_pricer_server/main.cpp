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


auto bond_pricer_request_processor = [] (ql_rest::json_weak_ptr pricer_request)
{
    auto request_id = pricer_request->get<std::string>("request_id");
    std::cout << "Request : " << request_id << std::endl;
    
    boost::property_tree::ptree pricer_results;
    pricer_results.put("request_id", request_id);
    
    try
    {
        auto business_date_in = pricer_request->get<std::string>("business_date");
        auto business_date = ql_rest::from_iso_string(business_date_in);

        QuantLibAddinCpp::qlSettingsSetEvaluationDate( QuantLib::BigInteger(business_date), OH_NULL );
        
        auto curve = pricer_request->get_child("curve");
        auto bond_template = curve.get_child("bond_template");
        auto curve_points = curve.get_child("curve_points");
    
        curve_builder cb( pricer_request->get<std::string>("request_id"), curve_points, bond_template);
        
        pricer_results.add_child("yield_curve", cb.get_yield_curve());
        
        auto curve_object = cb.get_curve_object();
        auto bond_engine = cb.get_bond_engine();
        
        auto instruments = pricer_request->get_child("instruments");
            
        for (auto it = instruments.begin(); it != instruments.end(); ++it)
        {
            auto instrument = it->first;
            
            auto bond_schedule = it->second.get_child("ust_bond_schedule");
            auto fixed_date_bond = it->second.get_child("ust_fixed_rate_bond");
         
            try
            {
                bond_pricer bp(bond_schedule, fixed_date_bond, bond_engine);
                
                boost::property_tree::ptree bond_pricer_results;
                bp.populate_pricing_results(bond_pricer_results);
                                        
                pricer_results.add_child(instrument, bond_pricer_results);
                
            } catch ( std::exception& exp )
            {
                std::cout << "Exception in bond setup Schedule : " << bond_schedule.get<std::string>("ObjectId")
                << " Fixed Rate Bond : " << fixed_date_bond.get<std::string>("ObjectId") << " : " << exp.what() << std::endl;
            }
        }
    }  catch (const boost::property_tree::ptree_error& e )
    {
        std::cout << "Exception : " << e.what() << std::endl;
        pricer_results.put("error_code", -1);
        pricer_results.put("error", e.what());
        
    } catch ( const std::exception& exp )
    {
        pricer_results.put("error_code", -2);
        pricer_results.put("error", exp.what());
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
