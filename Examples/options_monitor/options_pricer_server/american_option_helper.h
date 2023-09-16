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

#ifndef american_options_helper_h
#define american_options_helper_h

#include <qlrest/volatilities_reader.hpp>
#include <qlrest/payoffs_reader.hpp>
#include <qlrest/processes_reader.hpp>
#include <qlrest/exercise_reader.hpp>

namespace american_option_helper {

    auto options_pricer = [] (ql_rest::json_raw_ptr pricer_request, QuantLib::Date& settlement_date)
    {
        boost::property_tree::ptree pricer_results;
        
        auto vols_and_payoffs = pricer_request->get_child("vols_and_payoffs");
        auto exercise = pricer_request->get_child("exercise");
        auto idExercise = ql_rest::exercise::qlAmericanExercise(exercise);
        
        auto settlement_date_spot = settlement_date;
        QuantLib::Calendar settlement_calendar = QuantLib::TARGET();
        auto settlement_date_t_plus_1 = settlement_calendar.advance(settlement_date, 1, QuantLib::Days);
        
        auto pricer = [&]( std::string option_type )
        {
            auto options = vols_and_payoffs.get_child(option_type);
            
            boost::property_tree::ptree options_risk_and_npv;
       
            std::for_each(options.begin(), options.end(), [&]( ptree::value_type& pt ) {
            
                auto black_constant_vol = pt.second.get_child("qlBlackConstantVol");
                auto ql_striked_type_payoff = pt.second.get_child("qlStrikedTypePayoff");
                auto id_generalized_black_scholes_process = pt.second.get_child("qlGeneralizedBlackScholesProcess");
                
                QuantLib::ClosestRounding closest(3);
                boost::property_tree::ptree risk_and_npv;
        
                auto risk_free_rate = id_generalized_black_scholes_process.get<double>("RiskFreeRate");
                auto risk_free_rate_plus_1 = risk_free_rate + 1/100.0;
                
                auto volatility = black_constant_vol.get<double>("Volatility");
                auto volatility_plus_1 = volatility + 1/100.0;
                
                risk_and_npv.put("Strike",closest( ql_striked_type_payoff.get<double>("Strike") ) );
                risk_and_npv.put(option_type + "_vol", volatility);
                risk_and_npv.put("Underlying",closest( id_generalized_black_scholes_process.get<double>("Underlying") ) );
                risk_and_npv.put("RiskFreeRate", risk_free_rate);
                risk_and_npv.put("DividendYield", id_generalized_black_scholes_process.get<double>("DividendYield"));
                risk_and_npv.put("valuation_date", QuantLib::detail::iso_date_holder(settlement_date));
                risk_and_npv.put("expiration_date", exercise.get<std::string>("LatestDate"));
                
                
                auto option_setup = [&] (const std::string& object_id_prefix)
                {
                    black_constant_vol.put("SettlementDate", QuantLib::detail::iso_date_holder(settlement_date));
                    id_generalized_black_scholes_process.put("SettlementDate", QuantLib::detail::iso_date_holder(settlement_date));
                    
                    ql_striked_type_payoff.put("ObjectId", object_id_prefix + ql_striked_type_payoff.get<std::string>("ObjectId"));
                    auto idStrikedTypePayoff = ql_rest::payoffs::qlStrikedTypePayoff(ql_striked_type_payoff);
                
                    black_constant_vol.put("ObjectId", object_id_prefix + black_constant_vol.get<std::string>("ObjectId"));
                    auto idBlackVol = ql_rest::volatilities::qlBlackConstantVol(black_constant_vol);
                    
                    id_generalized_black_scholes_process.put("ObjectId", object_id_prefix + id_generalized_black_scholes_process.get<std::string>("ObjectId"));
                    id_generalized_black_scholes_process.put("BlackVolID", idBlackVol);
                    auto idGeneralizedBlackScholesProcess = ql_rest::processes::qlGeneralizedBlackScholesProcess(id_generalized_black_scholes_process);
                
                    std::string idPricingEngine = QuantLibAddinCpp::qlBinomialPricingEngine(object_id_prefix + idExercise + "engine", "CRR", idGeneralizedBlackScholesProcess, 200, OH_NULL, OH_NULL, true );
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
                id_generalized_black_scholes_process.put("RiskFreeRate", risk_free_rate_plus_1);
                auto vanilla_option_rho = option_setup("rho");
                id_generalized_black_scholes_process.put("RiskFreeRate", risk_free_rate);
                
                // vega
                black_constant_vol.put("Volatility", volatility_plus_1);
                auto vanilla_option_vega = option_setup("vega");
                black_constant_vol.put("Volatility", volatility);

                OH_GET_REFERENCE(ObjectIdLibObjPtr, vanilla_option_spot, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
                OH_GET_REFERENCE(ObjectIdLibObjThetaPtr, vanilla_option_theta, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
                OH_GET_REFERENCE(ObjectIdLibObjRhoPtr, vanilla_option_rho, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
                OH_GET_REFERENCE(ObjectIdLibObjVegaPtr, vanilla_option_vega, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);
                
                try {
                    risk_and_npv.put(option_type + "_npv",closest( ObjectIdLibObjPtr->NPV() ) );
                    risk_and_npv.put(option_type + "_delta", closest( ObjectIdLibObjPtr->delta() ) );
                    risk_and_npv.put(option_type + "_gamma", closest( ObjectIdLibObjPtr->gamma() ) );
                } catch ( ... )
                {
                }
                
                try {
                    risk_and_npv.put(option_type + "_theta", closest( ObjectIdLibObjThetaPtr->NPV() - ObjectIdLibObjPtr->NPV() ) );
                } catch (...)
                {
                }
                
                try {
                    risk_and_npv.put(option_type + "_rho", closest( ObjectIdLibObjRhoPtr->NPV() - ObjectIdLibObjPtr->NPV() ) );
                }
                catch (...)
                {
                }
                
                try {
                    risk_and_npv.put(option_type + "_vega", closest( ObjectIdLibObjVegaPtr->NPV() - ObjectIdLibObjPtr->NPV() ) );
                }
                catch (...)
                {
                }
                    
                options_risk_and_npv.push_back(std::make_pair("", risk_and_npv));
            });
            
            return options_risk_and_npv;
        };

        
        pricer_results.add_child("call", pricer("call"));
        pricer_results.add_child("put", pricer("put"));

        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pricer_results);
        
        std::cout << "Pricer Results : " << oss.str() << std::endl;
        
        pricer_results.put("error_code", 0);
        
        return pricer_results;
    };

}



#endif /* american_options_helper_h */
