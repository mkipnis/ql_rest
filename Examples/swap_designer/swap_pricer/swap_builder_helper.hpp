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

#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>
#include <oh/ohdefines.hpp>
#include <qlo/qladdin.hpp>
#include <Addins/Cpp/addincpp.hpp>

#include <boost/json.hpp>

namespace swap_designer
{

boost::shared_ptr<QuantLib::VanillaSwap> json_to_swap(  boost::json::object& swap_out_in, std::string& libor_index_with_curve, std::string& discouning_engine )
{
    auto fixed_leg_json = swap_out_in["fixed_leg"].as_object();
    auto floating_leg = swap_out_in["floating_leg"].as_object();
     
    QuantLib::Frequency FixedLegFrequencyEnum =
    ObjectHandler::Create<QuantLib::Frequency>()( boost::json::value_to<std::string>(fixed_leg_json["frequency"]) );
    
    QuantLib::Period periodFixedLeg = QuantLibAddin::periodFromFrequency( FixedLegFrequencyEnum );

    auto fixed_leg_schedule = QuantLibAddinCpp::qlSchedule(
                  "FixedSchedule",
                  ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(fixed_leg_json["start"])),
                  ql_rest::from_iso_string(boost::json::value_to<std::string>(fixed_leg_json["end"])),
                  QuantLibAddin::libraryToScalar( periodFixedLeg),
                  "TARGET",
                  boost::json::value_to<std::string>(fixed_leg_json["bdc"]),
                  boost::json::value_to<std::string>(fixed_leg_json["term_bdc"]),
                  boost::json::value_to<std::string>(fixed_leg_json["gen_rule"]),
                  false,
                  OH_NULL,
                  OH_NULL,
                  false,
                  false,
                  false);
 
    QuantLib::Frequency FloatingLegFrequencyEnum =
        ObjectHandler::Create<QuantLib::Frequency>()( boost::json::value_to<std::string>(floating_leg["frequency"]) );
    
    QuantLib::Period periodFloatingLeg = QuantLibAddin::periodFromFrequency( FloatingLegFrequencyEnum );

    auto floating_leg_schedule = QuantLibAddinCpp::qlSchedule(
                  "FloatingSchedule",
                  ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(floating_leg["start"])),
                  ql_rest::from_iso_string(boost::json::value_to<std::string>(floating_leg["end"]) ),
                  QuantLibAddin::libraryToScalar( periodFloatingLeg),
                  "TARGET",
                  boost::json::value_to<std::string>(floating_leg["bdc"]),
                  boost::json::value_to<std::string>(floating_leg["term_bdc"]),
                  boost::json::value_to<std::string>(floating_leg["gen_rule"]),
                  false,
                  OH_NULL,
                  OH_NULL,
                  false,
                  false,
                  false);
 
    auto vanilla_swap =  QuantLibAddinCpp::qlVanillaSwap ( "VanillaSwap",
                                                          boost::json::value_to<std::string>(swap_out_in["pay_or_recieve"]),
                                                          boost::json::value_to<double>(swap_out_in["notional"]),
                                                          fixed_leg_schedule,
                                                          boost::json::value_to<double>(swap_out_in["fixed_rate"]),
                                                          boost::json::value_to<std::string>(fixed_leg_json["day_counter"]),
                                                          floating_leg_schedule,
                                                          libor_index_with_curve,
                                                          boost::json::value_to<double>(swap_out_in["spread"]),
                                                          boost::json::value_to<std::string>(floating_leg["day_counter"]),
                                                          boost::json::value_to<std::string>(fixed_leg_json["bdc"]),
                                                                    false,
                                                                    false,
                                                                    false
                                                                   );
    
    OH_GET_REFERENCE(swap_ptr, vanilla_swap, QuantLibAddin::VanillaSwap, QuantLib::VanillaSwap)
    
    QuantLibAddinCpp::qlInstrumentSetPricingEngine( vanilla_swap,  discouning_engine, false );
    
    
    return swap_ptr;
    
};

boost::json::object swap_to_json( boost::shared_ptr<QuantLib::VanillaSwap> swap_ptr )
{
    auto fixed_leg_schedule = swap_ptr->fixedSchedule();
    auto float_leg_schedule = swap_ptr->floatingSchedule();
    
    boost::json::object swap_out;
    boost::json::object fixed_leg;
    boost::json::object floating_leg;
    
                                    
    fixed_leg["start"] =  ql_rest::from_ql_type_to_string( QuantLib::detail::iso_date_holder(fixed_leg_schedule.startDate()));
    fixed_leg["end"] = ql_rest::from_ql_type_to_string( QuantLib::detail::iso_date_holder(fixed_leg_schedule.endDate()));
    fixed_leg["frequency"] = ql_rest::from_ql_type_to_string(fixed_leg_schedule.tenor().frequency());
    fixed_leg["bdc"] = ql_rest::from_ql_type_to_string( fixed_leg_schedule.businessDayConvention() );
    fixed_leg["day_counter"] = ql_rest::from_ql_type_to_string( swap_ptr->fixedDayCount() );
    fixed_leg["gen_rule"] = ql_rest::from_ql_type_to_string( fixed_leg_schedule.rule() );
    fixed_leg["term_bdc"] = ql_rest::from_ql_type_to_string( fixed_leg_schedule.terminationDateBusinessDayConvention() );
    
    QuantLib::ClosestRounding closest(6);
    QuantLib::ClosestRounding closest_npv(2);
    
    auto fixedCoupons = swap_ptr->fixedLeg();
    boost::json::object fixed_leg_cashflows;
    
    for ( auto cashflow : fixedCoupons )
    {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::FixedRateCoupon>(cashflow);
        boost::json::object cash_flow_out;
                            
        cash_flow_out["accrual_start_date"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(coupon->accrualStartDate()));
        
        auto cashflow_date = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(coupon->date()));
        cash_flow_out["date"] = cashflow_date;
        cash_flow_out["amount"] = closest_npv(coupon->amount());
                                                
        fixed_leg_cashflows[cashflow_date] = cash_flow_out;
    }
    
    fixed_leg["cashflows"] = fixed_leg_cashflows;
    
    swap_out["fixed_leg"] = fixed_leg;
    
    floating_leg["start"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(float_leg_schedule.startDate()));
    floating_leg["end"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(float_leg_schedule.endDate()));
    floating_leg["frequency"] = ql_rest::from_ql_type_to_string(float_leg_schedule.tenor().frequency());
    floating_leg["bdc"] = ql_rest::from_ql_type_to_string( float_leg_schedule.businessDayConvention() );
    floating_leg["day_counter"] = ql_rest::from_ql_type_to_string(swap_ptr->floatingDayCount());
    floating_leg["gen_rule"] = ql_rest::from_ql_type_to_string( float_leg_schedule.rule() );
    floating_leg["term_bdc"] = ql_rest::from_ql_type_to_string( float_leg_schedule.terminationDateBusinessDayConvention() );
    
    auto floatingCoupons = swap_ptr->floatingLeg();
    boost::json::object floating_leg_cashflows;
    
    for ( auto cashflow : floatingCoupons )
    {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(cashflow);
        boost::json::object cash_flow_out;
                                                
        cash_flow_out["accrual_start_date"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(coupon->accrualStartDate()));
        
        auto cashflow_date = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(coupon->date()));
        cash_flow_out["date"] = cashflow_date;
        cash_flow_out["fixing_date"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder( coupon->fixingDate()));
        cash_flow_out["accrual_period"] = coupon->accrualPeriod();
        cash_flow_out["spread"] = coupon->spread();
        cash_flow_out["amount"] = closest_npv(coupon->amount());
                                                
        floating_leg_cashflows[cashflow_date] = cash_flow_out;
    }
    
    floating_leg["cashflows"] = floating_leg_cashflows;
    swap_out["floating_leg"] = floating_leg;
    
    swap_out["pay_or_recieve"] = ql_rest::from_ql_type_to_string(swap_ptr->type());
    swap_out["notional"] = swap_ptr->nominal();
    swap_out["fixed_rate"] = closest(swap_ptr->fixedRate());
    swap_out["spread"] = closest(swap_ptr->spread());

    swap_out["par_rate"] = closest(swap_ptr->fairRate());
    swap_out["par_spread"] = closest(swap_ptr->fairSpread());
    
    swap_out["floating_leg_npv"] = closest_npv(swap_ptr->floatingLegNPV());
    swap_out["fixed_leg_npv"] = closest_npv(swap_ptr->fixedLegNPV());
    swap_out["floating_leg_bps"] = closest_npv(swap_ptr->floatingLegBPS());
    swap_out["fixed_leg_bps"] = closest_npv(swap_ptr->fixedLegBPS());
    swap_out["npv"] = closest_npv(swap_ptr->NPV());
    
    return swap_out;
}

}
