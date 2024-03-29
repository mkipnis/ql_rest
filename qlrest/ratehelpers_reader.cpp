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

/* Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.body */

#include "ratehelpers_reader.hpp"

using namespace ql_rest;



std::string ratehelpers::qlBondHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlBondHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Price"].is_double() ? boost::json::value_to<double>(json_obj["Price"]) : boost::json::value_to<long>(json_obj["Price"]) ,
              boost::json::value_to<std::string>(json_obj["Bond"]),
              json_obj["UseCleanPrice"].as_bool(),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlDatedOISRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlDatedOISRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["StartDate"])),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["EndDate"])),
              json_obj["FixedRate"].is_double() ? boost::json::value_to<double>(json_obj["FixedRate"]) : boost::json::value_to<long>(json_obj["FixedRate"]) ,
              boost::json::value_to<std::string>(json_obj["ONIndex"]),
              boost::json::value_to<std::string>(json_obj["DiscountingCurve"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlDepositRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlDepositRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<std::string>(json_obj["IborIndex"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlDepositRateHelper2(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlDepositRateHelper2(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              boost::json::value_to<long>(json_obj["FixingDays"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              json_obj["EndOfMonth"].as_bool(),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFixedRateBondHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFixedRateBondHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Price"].is_double() ? boost::json::value_to<double>(json_obj["Price"]) : boost::json::value_to<long>(json_obj["Price"]) ,
              long(json_obj["SettlementDays"].as_int64()),
              json_obj["FaceAmount"].is_double() ? boost::json::value_to<double>(json_obj["FaceAmount"]) : boost::json::value_to<long>(json_obj["FaceAmount"]),
              boost::json::value_to<std::string>(json_obj["ScheduleID"]),
              ql_rest::vector_cast<double>(json_obj["Coupons"].as_array()),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
              boost::json::value_to<std::string>(json_obj["PaymentBDC"]),
              json_obj["Redemption"].is_double() ? boost::json::value_to<double>(json_obj["Redemption"]) : boost::json::value_to<long>(json_obj["Redemption"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["IssueDate"])),
              boost::json::value_to<std::string>(json_obj["PaymentCalendar"]),
              boost::json::value_to<std::string>(json_obj["ExCouponPeriod"]),
              boost::json::value_to<std::string>(json_obj["ExCouponCalendar"]),
              boost::json::value_to<std::string>(json_obj["ExCouponBDC"]),
              json_obj["ExCouponEndOfMonth"].as_bool(),
              json_obj["UseCleanPrice"].as_bool(),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFraRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFraRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<std::string>(json_obj["PeriodToStart"]),
              boost::json::value_to<std::string>(json_obj["IborIndex"]),
              boost::json::value_to<std::string>(json_obj["PillarDate"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["CustomPillarDate"])),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFraRateHelper2(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFraRateHelper2(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<std::string>(json_obj["PeriodToStart"]),
              boost::json::value_to<long>(json_obj["LengthInMonths"]),
              boost::json::value_to<long>(json_obj["FixingDays"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              json_obj["EndOfMonth"].as_bool(),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
              boost::json::value_to<std::string>(json_obj["PillarDate"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["CustomPillarDate"])),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFuturesRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFuturesRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Price"].is_double() ? boost::json::value_to<double>(json_obj["Price"]) : boost::json::value_to<long>(json_obj["Price"]) ,
              boost::json::value_to<std::string>(json_obj["FuturesType"]),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["FuturesDate"])),
              boost::json::value_to<std::string>(json_obj["IborIndex"]),
              json_obj["ConvexityAdjQuote"].is_double() ? boost::json::value_to<double>(json_obj["ConvexityAdjQuote"]) : boost::json::value_to<long>(json_obj["ConvexityAdjQuote"]) ,
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFuturesRateHelper2(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFuturesRateHelper2(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Price"].is_double() ? boost::json::value_to<double>(json_obj["Price"]) : boost::json::value_to<long>(json_obj["Price"]) ,
              boost::json::value_to<std::string>(json_obj["FuturesType"]),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["FuturesDate"])),
              boost::json::value_to<long>(json_obj["LengthInMonths"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              json_obj["EndOfMonth"].as_bool(),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
              json_obj["ConvexityAdjQuote"].is_double() ? boost::json::value_to<double>(json_obj["ConvexityAdjQuote"]) : boost::json::value_to<long>(json_obj["ConvexityAdjQuote"]) ,
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFuturesRateHelper3(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFuturesRateHelper3(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Price"].is_double() ? boost::json::value_to<double>(json_obj["Price"]) : boost::json::value_to<long>(json_obj["Price"]) ,
              boost::json::value_to<std::string>(json_obj["FuturesType"]),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["FuturesDate"])),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["EndDate"])),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
              json_obj["ConvexityAdjQuote"].is_double() ? boost::json::value_to<double>(json_obj["ConvexityAdjQuote"]) : boost::json::value_to<long>(json_obj["ConvexityAdjQuote"]) ,
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlFxSwapRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlFxSwapRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["FwdPoint"].is_double() ? boost::json::value_to<double>(json_obj["FwdPoint"]) : boost::json::value_to<long>(json_obj["FwdPoint"]) ,
              json_obj["SpotFx"].is_double() ? boost::json::value_to<double>(json_obj["SpotFx"]) : boost::json::value_to<long>(json_obj["SpotFx"]) ,
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              boost::json::value_to<long>(json_obj["FixingDays"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              json_obj["EndOfMonth"].as_bool(),
              json_obj["IsFxBaseCurrencyCollateralCurrency"].as_bool(),
              boost::json::value_to<std::string>(json_obj["CollateralCurve"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlOISRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlOISRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              boost::json::value_to<long>(json_obj["SettlDays"]),
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              json_obj["FixedRate"].is_double() ? boost::json::value_to<double>(json_obj["FixedRate"]) : boost::json::value_to<long>(json_obj["FixedRate"]) ,
              boost::json::value_to<std::string>(json_obj["ONIndex"]),
              boost::json::value_to<std::string>(json_obj["DiscountingCurve"]),
              json_obj["TelescopicValueDates"].as_bool(),
              boost::json::value_to<long>(json_obj["PaymentLag"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

double ratehelpers::qlRateHelperImpliedQuote(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlRateHelperImpliedQuote(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Trigger"].as_bool()
    );
}

double ratehelpers::qlRateHelperQuoteError(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlRateHelperQuoteError(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Trigger"].as_bool()
    );
}

double ratehelpers::qlRateHelperQuoteIsValid(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlRateHelperQuoteIsValid(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Trigger"].as_bool()
    );
}

std::string ratehelpers::qlRateHelperQuoteName(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlRateHelperQuoteName(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Trigger"].as_bool()
    );
}

double ratehelpers::qlRateHelperQuoteValue(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlRateHelperQuoteValue(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Trigger"].as_bool()
    );
}

std::string ratehelpers::qlSwapRateHelper(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlSwapRateHelper(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<std::string>(json_obj["SwapIndex"]),
              json_obj["Spread"].is_double() ? boost::json::value_to<double>(json_obj["Spread"]) : boost::json::value_to<long>(json_obj["Spread"]) ,
              boost::json::value_to<std::string>(json_obj["ForwardStart"]),
              boost::json::value_to<std::string>(json_obj["DiscountingCurve"]),
              boost::json::value_to<std::string>(json_obj["PillarDate"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["CustomPillarDate"])),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string ratehelpers::qlSwapRateHelper2(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlSwapRateHelper2(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["Rate"].is_double() ? boost::json::value_to<double>(json_obj["Rate"]) : boost::json::value_to<long>(json_obj["Rate"]) ,
              boost::json::value_to<long>(json_obj["SettlDays"]),
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["FixedLegFrequency"]),
              boost::json::value_to<std::string>(json_obj["FixedLegConvention"]),
              boost::json::value_to<std::string>(json_obj["FixedLegDayCounter"]),
              boost::json::value_to<std::string>(json_obj["IborIndex"]),
              json_obj["Spread"].is_double() ? boost::json::value_to<double>(json_obj["Spread"]) : boost::json::value_to<long>(json_obj["Spread"]) ,
              boost::json::value_to<std::string>(json_obj["ForwardStart"]),
              boost::json::value_to<std::string>(json_obj["DiscountingCurve"]),
              boost::json::value_to<std::string>(json_obj["PillarDate"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["CustomPillarDate"])),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}
