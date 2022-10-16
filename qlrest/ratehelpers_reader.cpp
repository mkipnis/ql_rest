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

/* Autogenerated from the following stub : gensrc/stubs/stub.qlrest.body */

#include "ratehelpers_reader.hpp"

using namespace ql_rest;



std::string ratehelpers::qlBondHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlBondHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Price")) ,
              pt.get<std::string>("Bond"),
              pt.get<bool>("UseCleanPrice"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlDatedOISRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlDatedOISRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::from_iso_string(pt.get<std::string>("StartDate") ),
              ql_rest::from_iso_string(pt.get<std::string>("EndDate") ),
              ObjectHandler::property_t(pt.get<std::string>("FixedRate")) ,
              pt.get<std::string>("ONIndex"),
              pt.get<std::string>("DiscountingCurve"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlDepositRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlDepositRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<std::string>("IborIndex"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlDepositRateHelper2(ptree const& pt)
{
    return QuantLibAddinCpp::qlDepositRateHelper2(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<std::string>("Tenor"),
              pt.get<long>("FixingDays"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("Convention"),
              pt.get<bool>("EndOfMonth"),
              pt.get<std::string>("DayCounter"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFixedRateBondHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlFixedRateBondHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Price")) ,
              pt.get<long>("SettlementDays"),
              pt.get<double>("FaceAmount"),
              pt.get<std::string>("ScheduleID"),
              ql_rest::vector_cast<double>(pt.get_child("Coupons")),
              pt.get<std::string>("DayCounter"),
              pt.get<std::string>("PaymentBDC"),
              pt.get<double>("Redemption"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("IssueDate")),
              pt.get<std::string>("PaymentCalendar"),
              pt.get<std::string>("ExCouponPeriod"),
              pt.get<std::string>("ExCouponCalendar"),
              pt.get<std::string>("ExCouponBDC"),
              pt.get<bool>("ExCouponEndOfMonth"),
              pt.get<bool>("UseCleanPrice"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFraRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlFraRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<std::string>("PeriodToStart"),
              pt.get<std::string>("IborIndex"),
              pt.get<std::string>("PillarDate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("CustomPillarDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFraRateHelper2(ptree const& pt)
{
    return QuantLibAddinCpp::qlFraRateHelper2(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<std::string>("PeriodToStart"),
              pt.get<long>("LengthInMonths"),
              pt.get<long>("FixingDays"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("Convention"),
              pt.get<bool>("EndOfMonth"),
              pt.get<std::string>("DayCounter"),
              pt.get<std::string>("PillarDate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("CustomPillarDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFuturesRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlFuturesRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Price")) ,
              pt.get<std::string>("FuturesType"),
              ql_rest::from_iso_string(pt.get<std::string>("FuturesDate") ),
              pt.get<std::string>("IborIndex"),
              ObjectHandler::property_t(pt.get<std::string>("ConvexityAdjQuote")) ,
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFuturesRateHelper2(ptree const& pt)
{
    return QuantLibAddinCpp::qlFuturesRateHelper2(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Price")) ,
              pt.get<std::string>("FuturesType"),
              ql_rest::from_iso_string(pt.get<std::string>("FuturesDate") ),
              pt.get<long>("LengthInMonths"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("Convention"),
              pt.get<bool>("EndOfMonth"),
              pt.get<std::string>("DayCounter"),
              ObjectHandler::property_t(pt.get<std::string>("ConvexityAdjQuote")) ,
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFuturesRateHelper3(ptree const& pt)
{
    return QuantLibAddinCpp::qlFuturesRateHelper3(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Price")) ,
              pt.get<std::string>("FuturesType"),
              ql_rest::from_iso_string(pt.get<std::string>("FuturesDate") ),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("EndDate")),
              pt.get<std::string>("DayCounter"),
              ObjectHandler::property_t(pt.get<std::string>("ConvexityAdjQuote")) ,
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlFxSwapRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlFxSwapRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("FwdPoint")) ,
              ObjectHandler::property_t(pt.get<std::string>("SpotFx")) ,
              pt.get<std::string>("Tenor"),
              pt.get<long>("FixingDays"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("Convention"),
              pt.get<bool>("EndOfMonth"),
              pt.get<bool>("IsFxBaseCurrencyCollateralCurrency"),
              pt.get<std::string>("CollateralCurve"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlOISRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlOISRateHelper(
         
              pt.get<std::string>("ObjectId"),
              pt.get<long>("SettlDays"),
              pt.get<std::string>("Tenor"),
              ObjectHandler::property_t(pt.get<std::string>("FixedRate")) ,
              pt.get<std::string>("ONIndex"),
              pt.get<std::string>("DiscountingCurve"),
              pt.get<bool>("TelescopicValueDates"),
              pt.get<long>("PaymentLag"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

double ratehelpers::qlRateHelperImpliedQuote(ptree const& pt)
{
    return QuantLibAddinCpp::qlRateHelperImpliedQuote(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}

double ratehelpers::qlRateHelperQuoteError(ptree const& pt)
{
    return QuantLibAddinCpp::qlRateHelperQuoteError(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}

double ratehelpers::qlRateHelperQuoteIsValid(ptree const& pt)
{
    return QuantLibAddinCpp::qlRateHelperQuoteIsValid(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}

std::string ratehelpers::qlRateHelperQuoteName(ptree const& pt)
{
    return QuantLibAddinCpp::qlRateHelperQuoteName(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}

double ratehelpers::qlRateHelperQuoteValue(ptree const& pt)
{
    return QuantLibAddinCpp::qlRateHelperQuoteValue(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}

std::string ratehelpers::qlSwapRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlSwapRateHelper(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<std::string>("SwapIndex"),
              ObjectHandler::property_t(pt.get<std::string>("Spread")) ,
              pt.get<std::string>("ForwardStart"),
              pt.get<std::string>("DiscountingCurve"),
              pt.get<std::string>("PillarDate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("CustomPillarDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string ratehelpers::qlSwapRateHelper2(ptree const& pt)
{
    return QuantLibAddinCpp::qlSwapRateHelper2(
         
              pt.get<std::string>("ObjectId"),
              ObjectHandler::property_t(pt.get<std::string>("Rate")) ,
              pt.get<long>("SettlDays"),
              pt.get<std::string>("Tenor"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("FixedLegFrequency"),
              pt.get<std::string>("FixedLegConvention"),
              pt.get<std::string>("FixedLegDayCounter"),
              pt.get<std::string>("IborIndex"),
              ObjectHandler::property_t(pt.get<std::string>("Spread")) ,
              pt.get<std::string>("ForwardStart"),
              pt.get<std::string>("DiscountingCurve"),
              pt.get<std::string>("PillarDate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("CustomPillarDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}