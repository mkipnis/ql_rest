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

#include "vanillaswap_reader.hpp"

using namespace ql_rest;



std::string vanillaswap::qlMakeIMMSwap(ptree const& pt)
{
    return QuantLibAddinCpp::qlMakeIMMSwap(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("SwapTenor"),
              pt.get<std::string>("IborIndex"),
              pt.get<double>("FixedRate"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("FirstImmDate")),
              pt.get<std::string>("FixDayCounter"),
              pt.get<double>("Spread"),
              pt.get<std::string>("PricingEngineID"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string vanillaswap::qlMakeVanillaSwap(ptree const& pt)
{
    return QuantLibAddinCpp::qlMakeVanillaSwap(
         
              pt.get<std::string>("ObjectId"),
              pt.get<long>("SettlDays"),
              pt.get<std::string>("SwapTenor"),
              pt.get<std::string>("IborIndex"),
              pt.get<double>("FixedRate"),
              pt.get<std::string>("ForwardStart"),
              pt.get<std::string>("FixDayCounter"),
              pt.get<double>("Spread"),
              pt.get<std::string>("PricingEngineID"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string vanillaswap::qlVanillaSwap(ptree const& pt)
{
    return QuantLibAddinCpp::qlVanillaSwap(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("PayerReceiver"),
              pt.get<double>("Nominal"),
              pt.get<std::string>("FixSchedule"),
              pt.get<double>("FixedRate"),
              pt.get<std::string>("FixDayCounter"),
              pt.get<std::string>("FloatingLegSchedule"),
              pt.get<std::string>("IborIndex"),
              pt.get<double>("Spread"),
              pt.get<std::string>("FloatingLegDayCounter"),
              pt.get<std::string>("PaymentConvention"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string vanillaswap::qlVanillaSwapFromSwapIndex(ptree const& pt)
{
    return QuantLibAddinCpp::qlVanillaSwapFromSwapIndex(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("SwapIndex"),
              ql_rest::from_iso_string(pt.get<std::string>("FixingDate") ),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string vanillaswap::qlVanillaSwapFromSwapRateHelper(ptree const& pt)
{
    return QuantLibAddinCpp::qlVanillaSwapFromSwapRateHelper(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("SwapRateHelper"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}
