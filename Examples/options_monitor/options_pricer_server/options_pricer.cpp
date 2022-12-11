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
#include <boost/lockfree/queue.hpp>
#include <qlrest/basic_microservice.hpp>

#include <iostream>
#include <tuple>

#include <qlrest/volatilities_reader.hpp>
#include <qlrest/payoffs_reader.hpp>
#include <qlrest/processes_reader.hpp>
#include <qlrest/exercise_reader.hpp>


#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>
#include <oh/ohdefines.hpp>
#include <qlo/qladdin.hpp>
#include <Addins/Cpp/addincpp.hpp>

//#include "swap_builder_helper.hpp"


auto options_pricer_request_processor = [] (ql_rest::json_weak_ptr pricer_request)
{
    boost::property_tree::ptree pricer_results;
    auto request_id = pricer_request->get<std::string>("request_id");
    
    try
     {
        pricer_results.put("request_id", request_id);
         
         // Let's adjust the business date to a valid valuation date
         auto business_date = ObjectHandler::Create<QuantLib::Calendar>() ( "TARGET" ).adjust(
                             QuantLib::DateParser::parseFormatted( pricer_request->get<std::string>("business_date"),"%Y-%m-%d"),
                             ObjectHandler::Create<QuantLib::BusinessDayConvention>() ( ObjectHandler::property_t("Modified Following") ) );
         
         QuantLibAddinCpp::qlSettingsSetEvaluationDate( (long) business_date.serialNumber(), OH_NULL);
         QuantLib::Calendar settlement_calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
         
         auto settlement_date = settlement_calendar.advance(business_date, 1, QuantLib::Days);
         
         std::cout << "Business Date : " << business_date << "Settlement Date : " << QuantLib::detail::iso_date_holder(settlement_date) << std::endl;
         
         auto vols_and_payoffs = pricer_request->get_child("vols_and_payoffs");
         
         auto idExercise = ql_rest::exercise::qlEuropeanExercise(pricer_request->get_child("exercise"));
         
         
         auto pricer = [&]( std::string option_type )
         {
             
             auto options = vols_and_payoffs.get_child(option_type);
             
             
             boost::property_tree::ptree options_risk_and_npv;
        
             std::for_each(options.begin(), options.end(), [&]( ptree::value_type& pt ) {
             
                 auto black_constant_vol = pt.second.get_child("qlBlackConstantVol");
                 black_constant_vol.put("SettlementDate", QuantLib::detail::iso_date_holder(settlement_date));
             
                 auto id_generalized_black_scholes_process = pt.second.get_child("qlGeneralizedBlackScholesProcess");
                 id_generalized_black_scholes_process.put("SettlementDate", QuantLib::detail::iso_date_holder(settlement_date));
             
                 auto ql_striked_type_payoff = pt.second.get_child("qlStrikedTypePayoff");
                 auto idStrikedTypePayoff = ql_rest::payoffs::qlStrikedTypePayoff(ql_striked_type_payoff);
             
                 ql_rest::volatilities::qlBlackConstantVol(black_constant_vol);
                 auto idGeneralizedBlackScholesProcess = ql_rest::processes::qlGeneralizedBlackScholesProcess(id_generalized_black_scholes_process);
             
                 std::string idPricingEngine = QuantLibAddinCpp::qlPricingEngine("engine", "AE", idGeneralizedBlackScholesProcess, OH_NULL, OH_NULL, true );
                 std::string idVanillaOption = QuantLibAddinCpp::qlVanillaOption("option", idStrikedTypePayoff, idExercise, OH_NULL,OH_NULL,true);

                 QuantLibAddinCpp::qlInstrumentSetPricingEngine(idVanillaOption, idPricingEngine, OH_NULL);

                 OH_GET_REFERENCE(ObjectIdLibObjPtr, idVanillaOption, QuantLibAddin::VanillaOption, QuantLib::VanillaOption);

                 std::cout << option_type << "_NPV : " << ql_striked_type_payoff.get<std::string>("Strike") << " : " <<  ObjectIdLibObjPtr->NPV()  << ":" << ObjectIdLibObjPtr->delta() << std::endl;
                 
                 QuantLib::ClosestRounding closest(3);
                 boost::property_tree::ptree risk_and_npv;

                 risk_and_npv.put("Strike",closest( ql_striked_type_payoff.get<double>("Strike") ) );
                 risk_and_npv.put(option_type + "_vol",black_constant_vol.get<double>("Volatility") );
                 risk_and_npv.put(option_type + "_npv",closest( ObjectIdLibObjPtr->NPV() ) );
                 risk_and_npv.put(option_type + "_delta", closest( ObjectIdLibObjPtr->delta() ) );
                 risk_and_npv.put(option_type + "_vega", closest( ObjectIdLibObjPtr->vega() ) );
                 risk_and_npv.put(option_type + "_gamma", closest( ObjectIdLibObjPtr->gamma() ) );
                 risk_and_npv.put(option_type + "_theta", closest( ObjectIdLibObjPtr->thetaPerDay() ) );
                 risk_and_npv.put(option_type + "_rho", closest( ObjectIdLibObjPtr->rho() ) );
        
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
                  
     } catch (const boost::property_tree::ptree_error& e )
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
    
    std::cout << "OptionsPricer" << std::endl;
    
    if ( argc != 3 )
    {
        std::cerr << "Usage: qlRestOptionsPricer <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto const address = boost::asio::ip::make_address( argv[1] );
    auto const port = static_cast<unsigned short>( std::atoi(argv[2]) );
    
    boost::asio::io_context ioc;
    
    
    try
    {
        std::make_shared<ql_rest::listener>( ioc, tcp::endpoint{address, port}, options_pricer_request_processor)->run();
        
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
