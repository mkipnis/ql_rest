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

#include "curve_builder.hpp"

#include <qlrest/schedule_reader.hpp>
#include <qlrest/bonds_reader.hpp>

#include <ql/quantlib.hpp>

#include <Addins/Cpp/ratehelpers.hpp>
#include <Addins/Cpp/pricingengines.hpp>
#include <Addins/Cpp/piecewiseyieldcurve.hpp>
#include <qlo/piecewiseyieldcurve.hpp>
#include <qlo/bonds.hpp>

#include <Addins/Cpp/addincpp.hpp>

#include <boost/json/src.hpp>
#include <boost/json/object.hpp>


curve_builder::curve_builder( std::string request_id,
                             const boost::json::object& curve_points,
                             boost::json::object& bond_template)
{
    auto fixed_rate_bond = bond_template["ust_fixed_rate_bond"];
    
    std::vector<std::string> rate_helper;
    std::vector<std::string> idList;
    
    for (auto it = curve_points.begin(); it != curve_points.end(); ++it)
    {
        auto tenor = std::string(it->key());
        auto rate = it->value().as_double();
        
        QuantLib::Period start_period;
        QuantLib::Period end_period;

        QuantLibAddin::cppToLibrary( "1D", start_period);
        QuantLibAddin::cppToLibrary( tenor, end_period);
        
        QuantLib::Date adjustedSettlmentDate = QuantLib::Date::todaysDate();
        
        QuantLib::Calendar ust_calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        QuantLib::Date startDate = ust_calendar.advance( adjustedSettlmentDate, start_period);
        QuantLib::Date endDate = ust_calendar.advance( startDate, end_period);

        std::string schedule_obj_id = request_id + std::string("/schedule/") + tenor;
        
        bond_template["ust_bond_schedule"].as_object()["ObjectId"] = schedule_obj_id;
        
        bond_template["ust_bond_schedule"].as_object()["EffectiveDate"] = ql_rest::from_ql_type_to_string( QuantLib::io::iso_date(startDate) );
        bond_template["ust_bond_schedule"].as_object()["TerminationDate"] = ql_rest::from_ql_type_to_string(QuantLib::io::iso_date(endDate));
        bond_template["ust_fixed_rate_bond"].as_object()["IssueDate"] = ql_rest::from_ql_type_to_string(QuantLib::io::iso_date(startDate));
                
        try
        {
            auto tmp_bond_template = bond_template["ust_fixed_rate_bond"].as_object();
            std::string bond_obj_id = request_id + std::string("/ust_fixed_rate_bond/") +  tenor;
            auto schedule_id = ql_rest::schedule::qlSchedule(bond_template["ust_bond_schedule"].as_object());
            
            tmp_bond_template["ObjectId"] = bond_obj_id;
            tmp_bond_template["ScheduleID"] = schedule_id;

            boost::json::array coupons;
            
            coupons.emplace_back(rate);
            
            tmp_bond_template["Coupons"].as_array() = coupons;
    
            auto fixed_date_bond = ql_rest::bonds::qlFixedRateBond(tmp_bond_template);
            std::string bond_rate_helper = std::string("qlBondHelper/") + fixed_date_bond;
            bond_rate_helper = QuantLibAddinCpp::qlBondHelper (bond_rate_helper, 100.0, fixed_date_bond, true, false, false,false);
            rate_helper.push_back(bond_rate_helper);
            
            idList.push_back(schedule_id);
            idList.push_back(fixed_date_bond);
            idList.push_back(bond_rate_helper);

        } catch( const std::exception& exp )
        {
            std::cout << "Unable to create bond schedule : " << exp.what() << std::endl;
        }
    };
    
    _piecewise_curve =
           QuantLibAddinCpp::qlPiecewiseYieldCurve (request_id + std::string("/qlPiecewiseYieldCurve"),
                                                    static_cast<long>(1),
                                                    "US government bond market",
                                                    rate_helper,
                                                    "Actual/Actual",
                                                    std::vector<ObjectHandler::property_t>(),
                                                    std::vector<ObjectHandler::property_t>(),
                                                    0.000000001,
                                                    "Discount",
                                                    "LogLinear",
                                                    false,
                                                    false,
                                                    false);
    
    
    typedef QuantLib::PiecewiseYieldCurve<QuantLib::Discount,QuantLib::LogLinear> curve_type;

    OH_GET_REFERENCE(yield_curve_lib_obj_ptr, _piecewise_curve, QuantLibAddin::PiecewiseYieldCurve, curve_type);

    yield_curve_lib_obj_ptr->enableExtrapolation();


    _bond_engine =
         QuantLibAddinCpp::qlBondEngine(request_id + std::string("/qlBondEngine"),
                                        _piecewise_curve,
                                        false,
                                        false,
                                        false);
    
}
