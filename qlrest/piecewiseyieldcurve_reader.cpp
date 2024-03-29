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

#include "piecewiseyieldcurve_reader.hpp"

using namespace ql_rest;



std::string piecewiseyieldcurve::qlPiecewiseYieldCurve(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlPiecewiseYieldCurve(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              boost::json::value_to<long>(json_obj["NDays"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              ql_rest::vector_cast<std::string>(json_obj["RateHelpers"].as_array()),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
             ql_rest::vector_cast<ObjectHandler::property_t,double>(json_obj["Jumps"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,long>(json_obj["JumpDates"].as_array()),
              json_obj["Accuracy"].is_double() ? json_obj["Accuracy"].as_double() : json_obj["Accuracy"].as_int64() ,
              boost::json::value_to<std::string>(json_obj["TraitsID"]),
              boost::json::value_to<std::string>(json_obj["InterpolatorID"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string piecewiseyieldcurve::qlPiecewiseYieldCurveMixedInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlPiecewiseYieldCurveMixedInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              boost::json::value_to<long>(json_obj["NDays"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              ql_rest::vector_cast<std::string>(json_obj["RateHelpers"].as_array()),
              boost::json::value_to<std::string>(json_obj["DayCounter"]),
             ql_rest::vector_cast<ObjectHandler::property_t,double>(json_obj["Jumps"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,long>(json_obj["JumpDates"].as_array()),
              json_obj["Accuracy"].is_double() ? json_obj["Accuracy"].as_double() : json_obj["Accuracy"].as_int64() ,
              boost::json::value_to<std::string>(json_obj["TraitsID"]),
              boost::json::value_to<std::string>(json_obj["InterpolatorID"]),
              boost::json::value_to<std::string>(json_obj["MixedInterpolationBehavior"]),
              long(json_obj["PillarsBeforeChange"].as_int64()),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}
