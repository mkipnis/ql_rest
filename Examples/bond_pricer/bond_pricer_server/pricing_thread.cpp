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

#include "curve_builder.hpp"
#include "common.h"

#include "curve_builder.hpp"
#include <qlrest/schedule_reader.hpp>
#include <qlrest/bonds_reader.hpp>

#include "bond_pricer.hpp"

pricing_thread::pricing_thread()
{
    QuantLibAddinCpp::initializeAddin();
    _pricer_queue = std::make_shared<json_ptr_pc_queue>(128);
};

void pricing_thread::run_pricer()
{
    std::cout << "Pricer thread" << std::endl;
    
    while (!this->_done.load())
    {
        
        json_raw_ptr pricer_request;
        while (_pricer_queue->pop(pricer_request))
        {
            auto request_id = pricer_request->get<std::string>("request_id");
            std::cout << "Request : " << request_id << std::endl;
            auto pricer_results = std::make_shared<boost::property_tree::ptree>();
            pricer_results->put("request_id", request_id);
            
            try
            {
                auto business_date_in = pricer_request->get<std::string>("business_date");
                auto business_date = ql_rest::from_iso_string(business_date_in);

                QuantLibAddinCpp::qlSettingsSetEvaluationDate( QuantLib::BigInteger(business_date), OH_NULL );
                
                auto curve = pricer_request->get_child("curve");
                auto bond_template = curve.get_child("bond_template");
                auto curve_points = curve.get_child("curve_points");
                // auto curve_date = curve.get_child("curve_date");
                //auto curve_out = curve.get_child("curve_out");
            
                curve_builder cb( pricer_request->get<std::string>("request_id"), curve_points, bond_template);
                
                pricer_results->add_child("yield_curve", cb.get_yield_curve());
                //pricer_results->add_child("curve_date", curve_date);

                
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
                                                
                        pricer_results->add_child(instrument, bond_pricer_results);
                        
                    } catch ( std::exception& exp )
                    {
                        std::cout << "Exception in bond setup Schedule : " << bond_schedule.get<std::string>("ObjectId")
                        << " Fixed Rate Bond : " << fixed_date_bond.get<std::string>("ObjectId") << " : " << exp.what() << std::endl;
                    }
                    
                    std::cout << "All set with schedule" << std::endl;
                }
            
                std::ostringstream oss;
                boost::property_tree::json_parser::write_json(oss, *pricer_results);
            
                std::cout << oss.str() << std::endl;
    
            } catch (const boost::property_tree::ptree_error& e )
            {
                std::cout << "Exception : " << e.what() << std::endl;
            }
            
            pricer_results->put("state", static_cast<int>(request_state_manager::request_state::processed));
            _request_state_manager.set_request_state(request_id, pricer_results);

            delete pricer_request;
            ObjectHandler::Repository::instance().deleteAllObjects(true);
        }
    }
};

void pricing_thread::stop()
{
   _done.store(true);
}

void pricing_thread::enqueue_request(std::string request_id, json_raw_ptr request_ptr)
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
