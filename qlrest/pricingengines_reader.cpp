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

// This file was generated automatically by qlrest_gensrc.py.  If you edit this file
// manually then your changes will be lost the next time gensrc runs.

/* Autogenerated from the following stub : ql_rest/gensrc/stubs/stub.qlrest.body */

#include "pricingengines_reader.hpp"

using namespace ql_rest;



std::string pricingengines::qlAnalyticCapFloorEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlAnalyticCapFloorEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("HandleModel"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBachelierCapFloorEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlBachelierCapFloorEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              pt.get<std::string>("VolTS"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBachelierCapFloorEngine2(ptree const& pt)
{
    return QuantLibAddinCpp::qlBachelierCapFloorEngine2(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              ObjectHandler::property_t(pt.get<std::string>("Vol")) ,
              pt.get<std::string>("DayCounter"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBinomialPricingEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlBinomialPricingEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("EngineID"),
              pt.get<std::string>("ProcessID"),
              pt.get<long>("TimeSteps"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackCalculator(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackCalculator(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("OptionType"),
              pt.get<double>("Strike"),
              pt.get<double>("AtmForwardValue"),
              pt.get<double>("StdDev"),
              pt.get<double>("Deflator"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackCalculator2(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackCalculator2(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("PayoffID"),
              pt.get<double>("AtmForwardValue"),
              pt.get<double>("StdDev"),
              pt.get<double>("Deflator"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackCapFloorEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackCapFloorEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              pt.get<std::string>("VolTS"),
              pt.get<double>("Displacement"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackCapFloorEngine2(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackCapFloorEngine2(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              ObjectHandler::property_t(pt.get<std::string>("Vol")) ,
              pt.get<double>("Displacement"),
              pt.get<std::string>("DayCounter"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackScholesCalculator(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackScholesCalculator(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("OptionType"),
              pt.get<double>("Strike"),
              pt.get<double>("Spot"),
              pt.get<double>("Growth"),
              pt.get<double>("StdDev"),
              pt.get<double>("Deflator"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackScholesCalculator2(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackScholesCalculator2(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("PayoffID"),
              pt.get<double>("Spot"),
              pt.get<double>("Growth"),
              pt.get<double>("StdDev"),
              pt.get<double>("Deflator"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackSwaptionEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackSwaptionEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              pt.get<std::string>("VolTS"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBlackSwaptionEngine2(ptree const& pt)
{
    return QuantLibAddinCpp::qlBlackSwaptionEngine2(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              ObjectHandler::property_t(pt.get<std::string>("Vol")) ,
              pt.get<double>("Displacement"),
              pt.get<std::string>("DayCounter"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlBondEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlBondEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlDiscountingSwapEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlDiscountingSwapEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("YieldCurve"),
              pt.get<bool>("IncludeSettlDate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("SettlementDate")),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("NpvDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlJamshidianSwaptionEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlJamshidianSwaptionEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("Model"),
              pt.get<std::string>("YieldCurve"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlModelG2SwaptionEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlModelG2SwaptionEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("Model"),
              pt.get<double>("Range"),
              pt.get<long>("Intervals"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlPricingEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlPricingEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("EngineID"),
              pt.get<std::string>("ProcessID"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string pricingengines::qlTreeSwaptionEngine(ptree const& pt)
{
    return QuantLibAddinCpp::qlTreeSwaptionEngine(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("Model"),
              pt.get<long>("Nsteps"),
              pt.get<std::string>("YieldCurve"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}
