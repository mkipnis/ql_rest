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


curve_builder::curve_builder( std::string request_id,
                             const boost::property_tree::ptree& curve_points,
                             boost::property_tree::ptree& bond_template)
{
    auto fixed_rate_bond = bond_template.get_child("ust_fixed_rate_bond");
    
    std::vector<std::string> rate_helper;
    std::vector<std::string> idList;
    
    for (auto it = curve_points.begin(); it != curve_points.end(); ++it)
    {
        std::string tenor = it->first;
        double rate = curve_points.get<double>(tenor);
        
        QuantLib::Period start_period;
        QuantLib::Period end_period;

        QuantLibAddin::cppToLibrary( "1D", start_period);
        QuantLibAddin::cppToLibrary( tenor, end_period);
        
        QuantLib::Date adjustedSettlmentDate = QuantLib::Date::todaysDate();
        
        QuantLib::Calendar ust_calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        QuantLib::Date startDate = ust_calendar.advance( adjustedSettlmentDate, start_period);
        QuantLib::Date endDate = ust_calendar.advance( startDate, end_period);

        /*
        std::cout<<QuantLib::io::short_date(startDate)<<std::endl;
        // This is in fact the default format
        std::cout<<QuantLib::io::iso_date(startDate)<<std::endl;
        // ISO date format
        std::cout<<QuantLib::io::iso_date(endDate)<<std::endl;
*/
        //boost::property_tree::ptree schedule = schedule_ptree;
        std::string schedule_obj_id = request_id + std::string("/schedule/") +  tenor;
        
        bond_template.get_child("ust_bond_schedule").put("ObjectId", schedule_obj_id);
        bond_template.get_child("ust_bond_schedule").put("EffectiveDate", QuantLib::io::iso_date(startDate));
        bond_template.get_child("ust_bond_schedule").put("TerminationDate", QuantLib::io::iso_date(endDate));
        bond_template.get_child("ust_fixed_rate_bond").put("IssueDate", QuantLib::io::iso_date(startDate));
                
        try
        {
            std::cout << "--------------------" << std::endl;

            auto tmp_bond_template = bond_template.get_child("ust_fixed_rate_bond");
            std::string bond_obj_id = request_id + std::string("/ust_fixed_rate_bond/") +  tenor;
            auto schedule_id = ql_rest::schedule::qlSchedule(bond_template.get_child("ust_bond_schedule"));
            
            tmp_bond_template.put("ObjectId", bond_obj_id);
            tmp_bond_template.put("ScheduleID", schedule_id);

            boost::property_tree::ptree coupons;
            boost::property_tree::ptree ptree_rate;
            
            ptree_rate.put_value(rate);
            
            std::cout << "Par Rate : " << rate << std::endl;
            
            coupons.push_back(std::make_pair("", ptree_rate));
                        
            tmp_bond_template.put_child("Coupons", coupons);
    
            auto fixed_date_bond = ql_rest::bonds::qlFixedRateBond(tmp_bond_template);
            std::string bond_rate_helper = std::string("qlBondHelper/") + fixed_date_bond;
            bond_rate_helper = QuantLibAddinCpp::qlBondHelper (bond_rate_helper, 100.0, fixed_date_bond, true, false, false,false);
            rate_helper.push_back(bond_rate_helper);
            
            idList.push_back(schedule_id);
            idList.push_back(fixed_date_bond);
            idList.push_back(bond_rate_helper);

            std::cout << "--------------------" << std::endl;

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
