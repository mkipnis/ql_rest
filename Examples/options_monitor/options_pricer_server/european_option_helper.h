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

#ifndef european_option_helper_h
#define european_option_helper_h

#include <qlrest/volatilities_reader.hpp>
#include <qlrest/payoffs_reader.hpp>
#include <qlrest/processes_reader.hpp>
#include <qlrest/exercise_reader.hpp>

namespace european_option_helper {


auto options_pricer = [] (ql_rest::json_raw_ptr pricer_request, QuantLib::Date& settlement_date)
{
    boost::json::object pricer_results;
        
    auto vols_and_payoffs = pricer_request->at("vols_and_payoffs").as_object();
    auto exercise = pricer_request->at("exercise").as_object();
    auto idExercise = ql_rest::exercise::qlEuropeanExercise(exercise);
    
    auto settlement_date_spot = settlement_date;
    QuantLib::Calendar settlement_calendar = QuantLib::TARGET();
    auto settlement_date_t_plus_1 = settlement_calendar.advance(settlement_date, 1, QuantLib::Days);
    
    auto pricer = [&]( std::string option_type )
    {
        auto options = vols_and_payoffs.at(option_type).as_array();
        
        boost::json::array options_risk_and_npv;
   
        std::for_each(options.begin(), options.end(), [&]( boost::json::value& json_vol ) {
        
            auto option_obj = json_vol.as_object();
            auto black_constant_vol = option_obj["qlBlackConstantVol"].as_object();
            auto ql_striked_type_payoff = option_obj["qlStrikedTypePayoff"].as_object();
            auto id_generalized_black_scholes_process = option_obj["qlGeneralizedBlackScholesProcess"].as_object();
            
            QuantLib::ClosestRounding closest(3);
            boost::json::object risk_and_npv;
    
            auto risk_free_rate = boost::json::value_to<double>(id_generalized_black_scholes_process["RiskFreeRate"]);
            auto risk_free_rate_plus_1 = risk_free_rate + 1/100.0;
            
            auto volatility = boost::json::value_to<double>(black_constant_vol["Volatility"]);
            auto volatility_plus_1 = volatility + 1/100.0;
            
            risk_and_npv["Strike"] = closest( boost::json::value_to<double>(ql_striked_type_payoff["Strike"]) );
            risk_and_npv[option_type + "_vol"] = volatility;
            risk_and_npv["Underlying"] = closest( boost::json::value_to<double>(id_generalized_black_scholes_process["Underlying"]) );
            risk_and_npv["RiskFreeRate"] = risk_free_rate;
            risk_and_npv["DividendYield"] = boost::json::value_to<double>(id_generalized_black_scholes_process["DividendYield"]);
            risk_and_npv["valuation_date"] = ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(settlement_date));
            risk_and_npv["expiration_date"] = exercise["ExpiryDate"].as_string();
                        
            auto option_setup = [&] (const std::string& object_id_prefix)
            {
                black_constant_vol["SettlementDate"] =  ql_rest::from_ql_type_to_string(QuantLib::detail::iso_date_holder(settlement_date));
                
                id_generalized_black_scholes_process["SettlementDate"] = ql_rest::from_ql_type_to_string( QuantLib::detail::iso_date_holder(settlement_date));
                
                ql_striked_type_payoff["ObjectId"] = object_id_prefix + boost::json::value_to<std::string>(ql_striked_type_payoff["ObjectId"]);
                auto idStrikedTypePayoff = ql_rest::payoffs::qlStrikedTypePayoff(ql_striked_type_payoff);
            
                black_constant_vol["ObjectId"] = object_id_prefix + boost::json::value_to<std::string>(black_constant_vol["ObjectId"]);
                auto idBlackVol = ql_rest::volatilities::qlBlackConstantVol(black_constant_vol);
                
                id_generalized_black_scholes_process["ObjectId"] = object_id_prefix + boost::json::value_to<std::string>(id_generalized_black_scholes_process["ObjectId"]);
                
                id_generalized_black_scholes_process["BlackVolID"] = idBlackVol;
                auto idGeneralizedBlackScholesProcess = ql_rest::processes::qlGeneralizedBlackScholesProcess(id_generalized_black_scholes_process);
            
                std::string idPricingEngine = QuantLibAddinCpp::qlPricingEngine("engine", "AE", idGeneralizedBlackScholesProcess, OH_NULL, OH_NULL, true );
                std::string idVanillaOption = QuantLibAddinCpp::qlVanillaOption(object_id_prefix + idExercise + "option", idStrikedTypePayoff, idExercise, OH_NULL,OH_NULL,true);

                QuantLibAddinCpp::qlInstrumentSetPricingEngine(idVanillaOption, idPricingEngine, OH_NULL);
                
                return idVanillaOption;
            };
            
            auto vanilla_option_spot = option_setup("spot");
            
            // theta
            settlement_date = settlement_date_t_plus_1;
            auto vanilla_option_theta = option_setup("theta");
            settlement_date = settlement_date_spot;
            
            // rho
            id_generalized_black_scholes_process["RiskFreeRate"] = risk_free_rate_plus_1;
            auto vanilla_option_rho = option_setup("rho");
            id_generalized_black_scholes_process["RiskFreeRate"] = risk_free_rate;
            
            // vega
            black_constant_vol["Volatility"] = volatility_plus_1;
            auto vanilla_option_vega = option_setup("vega");
            black_constant_vol["Volatility"] = volatility;

            OH_GET_REFERENCE(ObjectIdLibObjPtr, vanilla_option_spot, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
            OH_GET_REFERENCE(ObjectIdLibObjThetaPtr, vanilla_option_theta, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
            OH_GET_REFERENCE(ObjectIdLibObjRhoPtr, vanilla_option_rho, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
            OH_GET_REFERENCE(ObjectIdLibObjVegaPtr, vanilla_option_vega, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
            
            try {
                risk_and_npv[option_type + "_npv"] = closest( ObjectIdLibObjPtr->NPV() );
                
                if ( !boost::math::isnan(ObjectIdLibObjPtr->delta()) )
                {
                    risk_and_npv[option_type + "_delta"] = closest( ObjectIdLibObjPtr->delta() );
                    risk_and_npv[option_type + "_gamma"] = closest( ObjectIdLibObjPtr->gamma() );
                }
            } catch ( ... )
            {
            }
            
            try {
                risk_and_npv[option_type + "_theta"] = closest( ObjectIdLibObjThetaPtr->NPV() - ObjectIdLibObjPtr->NPV() );
            } catch (...)
            {
            }
            
            try {
                risk_and_npv[option_type + "_rho"] = closest( ObjectIdLibObjRhoPtr->NPV() - ObjectIdLibObjPtr->NPV() );
            }
            catch (...)
            {
            }
            
            try {
                risk_and_npv[option_type + "_vega"] = closest( ObjectIdLibObjVegaPtr->NPV() - ObjectIdLibObjPtr->NPV() );
            }
            catch (...)
            {
            }
                
            options_risk_and_npv.emplace_back(risk_and_npv);
        });
        
        return options_risk_and_npv;
    };

    
    pricer_results["call"] = pricer("call");
    pricer_results["put"] = pricer("put");
    
    
    pricer_results["error_code"] = 0;
    
    std::cout << "Response Out: " << pricer_results << std::endl;
    
    return pricer_results;
};

}


#endif /* Header_h */
