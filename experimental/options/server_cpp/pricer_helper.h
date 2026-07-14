// Copyright (c) Mike Kipnis

#pragma once

#include <ql/quantlib.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace QuantLib;

namespace options_pricer {

    json::object american_option_with_greeks(
                                         const Calendar& calendar,
                                         const Date& today,
                                         const Date& maturity,
                                         double spot = 100.0,
                                         double strike = 100.0,
                                         double rate = 0.05,
                                         double dividend = 0.02,
                                         double volatility = 0.20,
                                         const std::string& option_type = "put") {
    
        Option::Type optType = (option_type == "call" || option_type == "CALL")
            ? Option::Call : Option::Put;
    
        auto payoff = ext::make_shared<PlainVanillaPayoff>(optType, strike);
    
        auto exercise = ext::make_shared<AmericanExercise>(today, maturity);
    
        VanillaOption option(payoff, exercise);
    
        // Market data
        Handle<Quote> spotHandle( ext::make_shared<SimpleQuote>(spot));
    
        DayCounter dayCounter = Actual365Fixed();
    
        Handle<YieldTermStructure> riskFreeCurve( ext::make_shared<FlatForward>( today, rate, dayCounter));
        Handle<YieldTermStructure> dividendCurve( ext::make_shared<FlatForward>( today, dividend, dayCounter));
    
        // Volatility quote for numerical vega
        auto volQuote = ext::make_shared<SimpleQuote>(volatility);
    
        Handle<BlackVolTermStructure> volHandle(
                                            ext::make_shared<BlackConstantVol>(
                                                                               today,
                                                                               calendar,
                                                                               Handle<Quote>(volQuote),
                                                                               dayCounter));
    
        auto process =
            ext::make_shared<BlackScholesMertonProcess>(
                                                spotHandle,
                                                dividendCurve,
                                                riskFreeCurve,
                                                volHandle);
    
        auto engine =
            ext::make_shared<FdBlackScholesVanillaEngine>(process);
    
        option.setPricingEngine(engine);
    
        // Numerical Vega
        auto numerical_vega =
        [&](double bump = 0.0001) -> double {
        
            double originalVol = volQuote->value();
        
            volQuote->setValue(originalVol + bump);
            double npvUp = option.NPV();
        
            volQuote->setValue(originalVol - bump);
            double npvDown = option.NPV();
        
            volQuote->setValue(originalVol);
        
            return (npvUp - npvDown) / (2.0 * bump);
        };
    
        // Numerical Rho
        auto numerical_rho =
        [&](double bump = 0.0001) -> double {
        
            Handle<YieldTermStructure> bumpedCurve(
                                               ext::make_shared<FlatForward>(
                                                                             today,
                                                                             rate + bump,
                                                                             dayCounter));
        
            auto bumpedProcess =
            ext::make_shared<BlackScholesMertonProcess>(
                                                    spotHandle,
                                                    dividendCurve,
                                                    bumpedCurve,
                                                    volHandle);
        
            auto bumpedEngine =
            ext::make_shared<FdBlackScholesVanillaEngine>(
                                                      bumpedProcess);
        
            option.setPricingEngine(bumpedEngine);
        
            double npvUp = option.NPV();
        
            // Restore original engine
            option.setPricingEngine(engine);
        
            double npvOriginal = option.NPV();
        
            return (npvUp - npvOriginal) / bump;
        };
    
        json::object result;
    
        result["price"] = std::round(option.NPV() * 100.0) / 100.0;
        result["delta"] = std::round(option.delta() * 100.0) / 100.0;
        result["gamma"] = std::round(option.gamma() * 10000.0) / 10000.0;
        result["vega"] = std::round(numerical_vega() * 10000.0) / 10000.0;
        result["theta"] = std::round(option.theta() * 10000.0) / 10000.0;
        result["rho"] = std::round(numerical_rho() * 10000.0) / 10000.0;
    
        return result;
    }

    Calendar CALENDAR = UnitedStates(UnitedStates::NYSE);

        json::object process_price_request(const json::value& request) {
    
        
        std::string symbol = boost::json::value_to<std::string>(request.at("symbol"));
        std::string exp_date = boost::json::value_to<std::string>(request.at("exp_date"));
        
        auto price = boost::json::value_to<double>( request.at("price"));
        auto risk_free_rate = boost::json::value_to<double>( request.at("risk_free_rate") );
        auto dividend_yield = boost::json::value_to<double>( request.at("dividend_yield") );
        
        Date maturity = DateParser::parseISO(exp_date);
    
        Date today = Date::todaysDate();
        
        const auto& vols = request.at("vols").as_object();
        const auto& calls = vols.at("calls").as_array();
        const auto& puts = vols.at("puts").as_array();
            
        json::object results {
                {"calls", json::array{}},
                {"puts",  json::array{}}
        };
    
        // Calls
        for (const auto& call : calls) {
        
            const auto& call_md = call.as_object();
            
            const auto strike = boost::json::value_to<double>(call_md.at("strike"));
            const auto vol = boost::json::value_to<double>(call_md.at("vol"));
            
            auto res =
                american_option_with_greeks(
                                    CALENDAR,
                                    today,
                                    maturity,
                                    price,
                                    strike,
                                    risk_free_rate,
                                    dividend_yield,
                                    vol / 100.0,
                                    "call");
        
            res["strike"] = strike;
            results["calls"].as_array().push_back(res);
        }
    
        // Puts
        for (const auto& put : puts) {

            const auto& put_md = put.as_object();
            
            const auto strike = boost::json::value_to<double>(put_md.at("strike"));
            const auto vol = boost::json::value_to<double>(put_md.at("vol"));
            
            auto res =
                american_option_with_greeks(
                                    CALENDAR,
                                    today,
                                    maturity,
                                    price,
                                    strike,
                                    risk_free_rate,
                                    dividend_yield,
                                    vol / 100.0,
                                    "put");
            
            res["strike"] = strike;
            results["puts"].as_array().push_back(res);
        }
    
        return results;
    }
}
