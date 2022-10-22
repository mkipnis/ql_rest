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

namespace swap_designer
{

boost::shared_ptr<QuantLib::VanillaSwap> json_to_swap( boost::property_tree::ptree& swap_out_in, std::string& libor_index_with_curve, std::string& discouning_engine )
{
    boost::property_tree::ptree fixed_leg_json = swap_out_in.get_child("fixed_leg");
    boost::property_tree::ptree floating_leg = swap_out_in.get_child("floating_leg");
     
    QuantLib::Frequency FixedLegFrequencyEnum =
        ObjectHandler::Create<QuantLib::Frequency>()( fixed_leg_json.get<std::string>("frequency") );
    
    QuantLib::Period periodFixedLeg = QuantLibAddin::periodFromFrequency( FixedLegFrequencyEnum );

    
    auto fixed_leg_schedule = QuantLibAddinCpp::qlSchedule(
                  "FixedSchedule",
                  ql_rest::from_iso_string_to_oh_property(fixed_leg_json.get<std::string>("start")),
                  ql_rest::from_iso_string(fixed_leg_json.get<std::string>("end") ),
                  QuantLibAddin::libraryToScalar( periodFixedLeg),
                  "TARGET",
                                                           fixed_leg_json.get<std::string>("bdc"),
                                                           fixed_leg_json.get<std::string>("term_bdc"),
                                                           fixed_leg_json.get<std::string>("gen_rule"),
                  false,
                  OH_NULL,
                  OH_NULL,
                  false,
                  false,
                  false);
    
    QuantLib::Frequency FloatingLegFrequencyEnum =
        ObjectHandler::Create<QuantLib::Frequency>()( floating_leg.get<std::string>("frequency") );
    
    QuantLib::Period periodFloatingLeg = QuantLibAddin::periodFromFrequency( FloatingLegFrequencyEnum );

    
    auto floating_leg_schedule = QuantLibAddinCpp::qlSchedule(
                  "FloatingSchedule",
                  ql_rest::from_iso_string_to_oh_property(floating_leg.get<std::string>("start")),
                  ql_rest::from_iso_string(floating_leg.get<std::string>("end") ),
                  QuantLibAddin::libraryToScalar( periodFloatingLeg),
                  "TARGET",
                                                              floating_leg.get<std::string>("bdc"),
                                                              floating_leg.get<std::string>("term_bdc"),
                                                              floating_leg.get<std::string>("gen_rule"),
                  false,
                  OH_NULL,
                  OH_NULL,
                  false,
                  false,
                  false);
        
    
    auto vanilla_swap =  QuantLibAddinCpp::qlVanillaSwap ( "VanillaSwap",
                                                          swap_out_in.get<std::string>("pay_or_recieve"),
                                                          swap_out_in.get<double>("notional"),
                                                                   fixed_leg_schedule,
                                                          swap_out_in.get<double>("fixed_rate"),
                                                                    fixed_leg_json.get<std::string>("day_counter"),
                                                                    floating_leg_schedule,
                                                                    libor_index_with_curve,
                                                            swap_out_in.get<double>("spread"),
                                                                    floating_leg.get<std::string>("day_counter"),
                                                                    fixed_leg_json.get<std::string>("bdc"),
                                                                    false,
                                                                    false,
                                                                    false
                                                                   );
    
    OH_GET_REFERENCE(swap_ptr, vanilla_swap, QuantLibAddin::VanillaSwap, QuantLib::VanillaSwap)
    
    QuantLibAddinCpp::qlInstrumentSetPricingEngine( vanilla_swap,  discouning_engine, false );
    
    return swap_ptr;
    
};

boost::property_tree::ptree swap_to_json( boost::shared_ptr<QuantLib::VanillaSwap> swap_ptr )
{
    auto fixed_leg_schedule = swap_ptr->fixedSchedule();
    auto float_leg_schedule = swap_ptr->floatingSchedule();
    
    boost::property_tree::ptree swap_out;
    boost::property_tree::ptree fixed_leg;
    boost::property_tree::ptree floating_leg;
    
                                    
    fixed_leg.put("start", QuantLib::detail::iso_date_holder(fixed_leg_schedule.startDate()) );
    fixed_leg.put("end", QuantLib::detail::iso_date_holder(fixed_leg_schedule.endDate()) );
    fixed_leg.put("frequency", fixed_leg_schedule.tenor().frequency() );
    fixed_leg.put("bdc", fixed_leg_schedule.businessDayConvention() );
    fixed_leg.put("day_counter", swap_ptr->fixedDayCount() );
    fixed_leg.put("gen_rule", fixed_leg_schedule.rule() );
    fixed_leg.put("term_bdc", fixed_leg_schedule.terminationDateBusinessDayConvention() );
    
    QuantLib::ClosestRounding closest(6);
    QuantLib::ClosestRounding closest_npv(2);
    
    auto fixedCoupons = swap_ptr->fixedLeg();
    boost::property_tree::ptree fixed_leg_cashflows;
    
    for ( auto cashflow : fixedCoupons )
    {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::FixedRateCoupon>(cashflow);
        boost::property_tree::ptree cash_flow_out;
                            
        cash_flow_out.put("accrual_start_date", QuantLib::detail::iso_date_holder(coupon->accrualStartDate()));
        cash_flow_out.put("date", QuantLib::detail::iso_date_holder(coupon->date()));
        cash_flow_out.put("amount", closest_npv(coupon->amount()));
                                                
        fixed_leg_cashflows.add_child(cash_flow_out.get<std::string>("date"), cash_flow_out);
    }
    
    fixed_leg.add_child("cashflows", fixed_leg_cashflows);
    
    swap_out.add_child("fixed_leg", fixed_leg);
    
    floating_leg.put("start", QuantLib::detail::iso_date_holder(float_leg_schedule.startDate()) );
    floating_leg.put("end", QuantLib::detail::iso_date_holder(float_leg_schedule.endDate()) );
    floating_leg.put("frequency", float_leg_schedule.tenor().frequency() );
    floating_leg.put("bdc", float_leg_schedule.businessDayConvention() );
    floating_leg.put("day_counter", swap_ptr->floatingDayCount() );
    floating_leg.put("gen_rule", float_leg_schedule.rule() );
    floating_leg.put("term_bdc", float_leg_schedule.terminationDateBusinessDayConvention() );
    
    auto floatingCoupons = swap_ptr->floatingLeg();
    boost::property_tree::ptree floating_leg_cashflows;
    
    
    for ( auto cashflow : floatingCoupons )
    {
        auto coupon = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(cashflow);
        boost::property_tree::ptree cash_flow_out;
                                                
        cash_flow_out.put("accrual_start_date", QuantLib::detail::iso_date_holder(coupon->accrualStartDate()));
        cash_flow_out.put("date", QuantLib::detail::iso_date_holder(coupon->date()));
        cash_flow_out.put("fixing_date", QuantLib::detail::iso_date_holder( coupon->fixingDate()));
        cash_flow_out.put("accrual_period",coupon->accrualPeriod());
        cash_flow_out.put("spread",coupon->spread());
        cash_flow_out.put("amount",closest_npv(coupon->amount()));
                                                
        floating_leg_cashflows.add_child(cash_flow_out.get<std::string>("date"), cash_flow_out);
    }
    
    floating_leg.add_child("cashflows", floating_leg_cashflows);
    swap_out.add_child("floating_leg", floating_leg);
    
    swap_out.put("pay_or_recieve", swap_ptr->type());
    swap_out.put("notional", swap_ptr->nominal());
    swap_out.put("fixed_rate", closest(swap_ptr->fixedRate()));
    swap_out.put("spread", closest(swap_ptr->spread()));

    swap_out.put("par_rate", closest(swap_ptr->fairRate()));
    swap_out.put("par_spread" , closest(swap_ptr->fairSpread()));
    
    swap_out.put("floating_leg_npv", closest_npv(swap_ptr->floatingLegNPV()));
    swap_out.put("fixed_leg_npv",closest_npv(swap_ptr->fixedLegNPV()));
    swap_out.put("floating_leg_bps",closest_npv(swap_ptr->floatingLegBPS()));
    swap_out.put("fixed_leg_bps",closest_npv(swap_ptr->fixedLegBPS()));
    swap_out.put("npv",closest_npv(swap_ptr->NPV()));
    
    return swap_out;
}

}
