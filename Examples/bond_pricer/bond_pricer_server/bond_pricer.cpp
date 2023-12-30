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

#include "bond_pricer.hpp"


#include <ql/quantlib.hpp>
#include <Addins/Cpp/addincpp.hpp>
#include <oh/ohdefines.hpp>

#include <qlrest/qlrest_common.h>

#include <qlrest/schedule_reader.hpp>
#include <qlrest/bonds_reader.hpp>

#include <Addins/Cpp/addincpp.hpp>


bond_pricer::bond_pricer(  boost::json::object& schedule,
                          boost::json::object& fixed_rate_bond,
                         std::string& pricing_engine ) :
                        _pricing_engine(pricing_engine)
{
    auto schedule_out = ql_rest::schedule::qlSchedule(schedule);
    _bond_object = ql_rest::bonds::qlFixedRateBond(fixed_rate_bond);
};

bool bond_pricer::populate_pricing_results(boost::json::object& results)
{
    try
    {
        QuantLibAddinCpp::qlInstrumentSetPricingEngine( _bond_object, _pricing_engine, false );
    
        OH_GET_REFERENCE(fixed_rate_bond_lib_obj_ptr, _bond_object, QuantLibAddin::FixedRateBond, QuantLib::FixedRateBond);
    
        auto ai = fixed_rate_bond_lib_obj_ptr->accruedAmount();
        auto clean_price = fixed_rate_bond_lib_obj_ptr->cleanPrice();
        auto dirty_price = fixed_rate_bond_lib_obj_ptr->dirtyPrice();
        auto yield = fixed_rate_bond_lib_obj_ptr->yield(fixed_rate_bond_lib_obj_ptr->dayCounter (), QuantLib::CompoundedThenSimple, fixed_rate_bond_lib_obj_ptr->frequency());
        
        QuantLib::InterestRate yield_rate(yield, fixed_rate_bond_lib_obj_ptr->dayCounter(), QuantLib::Compounded, fixed_rate_bond_lib_obj_ptr->frequency());
    
        auto dv01 = QuantLib::BondFunctions::basisPointValue(*fixed_rate_bond_lib_obj_ptr, yield_rate );
        auto ai_days = QuantLib::BondFunctions::accruedDays(*fixed_rate_bond_lib_obj_ptr);
        auto modified_duration = QuantLib::BondFunctions::duration(*fixed_rate_bond_lib_obj_ptr, yield_rate);
        auto convexity =  QuantLib::BondFunctions::convexity(*fixed_rate_bond_lib_obj_ptr, yield_rate);
        
        results["BondName"] = _bond_object;
        results["CleanPrice"] = clean_price;
        results["DirtyPrice"] = dirty_price;
        results["AI"] = ai;
        results["AI(Days)"] = ai_days;
        results["modified_duration"] = modified_duration;
        results["convexity"] = convexity;
        results["Yield"] = yield;
        results["DV01"] = dv01;
        
    } catch ( std::exception& exp )
    {
        std::cout << "Error: " << exp.what();
        
        return false;
    }
    
    return true;
}
