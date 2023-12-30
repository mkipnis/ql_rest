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

#include "interpolation_reader.hpp"

using namespace ql_rest;



std::string interpolation::qlAbcdInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlAbcdInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::vector_cast<double>(json_obj["XArray"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(json_obj["YArray"].as_array()),
              json_obj["A"].is_double() ? json_obj["A"].as_double() : json_obj["A"].as_int64() ,
              json_obj["B"].is_double() ? json_obj["B"].as_double() : json_obj["B"].as_int64() ,
              json_obj["C"].is_double() ? json_obj["C"].as_double() : json_obj["C"].as_int64() ,
              json_obj["D"].is_double() ? json_obj["D"].as_double() : json_obj["D"].as_int64() ,
              json_obj["AIsFixed"].as_bool(),
              json_obj["BIsFixed"].as_bool(),
              json_obj["CIsFixed"].as_bool(),
              json_obj["DIsFixed"].as_bool(),
              json_obj["VegaWeighted"].as_bool(),
              boost::json::value_to<std::string>(json_obj["EndCriteria"]),
              boost::json::value_to<std::string>(json_obj["OptimizationMeth"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string interpolation::qlCubicInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlCubicInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::vector_cast<double>(json_obj["XArray"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(json_obj["YArray"].as_array()),
              boost::json::value_to<std::string>(json_obj["DerApprox"]),
              json_obj["Monotonic"].as_bool(),
              boost::json::value_to<std::string>(json_obj["LeftConditionType"]),
              json_obj["LeftConditionValue"].is_double() ? json_obj["LeftConditionValue"].as_double() : json_obj["LeftConditionValue"].as_int64() ,
              boost::json::value_to<std::string>(json_obj["RightConditionType"]),
              json_obj["RightConditionValue"].is_double() ? json_obj["RightConditionValue"].as_double() : json_obj["RightConditionValue"].as_int64() ,
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

bool interpolation::qlExtrapolatorEnableExtrapolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlExtrapolatorEnableExtrapolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              json_obj["ExtrapolationFlag"].as_bool(),
              json_obj["Trigger"].as_bool()
    );
}

std::string interpolation::qlInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              boost::json::value_to<std::string>(json_obj["InterpolationType"]),
              ql_rest::vector_cast<double>(json_obj["XArray"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(json_obj["YArray"].as_array()),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string interpolation::qlMixedLinearCubicInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlMixedLinearCubicInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::vector_cast<double>(json_obj["XArray"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(json_obj["YArray"].as_array()),
              long(json_obj["SwitchIndex"].as_int64()),
              boost::json::value_to<std::string>(json_obj["MixedInterpolationBehavior"]),
              boost::json::value_to<std::string>(json_obj["DerApprox"]),
              json_obj["Monotonic"].as_bool(),
              boost::json::value_to<std::string>(json_obj["LeftConditionType"]),
              json_obj["LeftConditionValue"].is_double() ? json_obj["LeftConditionValue"].as_double() : json_obj["LeftConditionValue"].as_int64() ,
              boost::json::value_to<std::string>(json_obj["RightConditionType"]),
              json_obj["RightConditionValue"].is_double() ? json_obj["RightConditionValue"].as_double() : json_obj["RightConditionValue"].as_int64() ,
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string interpolation::qlSABRInterpolation(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlSABRInterpolation(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::vector_cast<double>(json_obj["XArray"].as_array()),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(json_obj["YArray"].as_array()),
              json_obj["Expiry"].is_double() ? boost::json::value_to<double>(json_obj["Expiry"]) : boost::json::value_to<long>(json_obj["Expiry"]),
              json_obj["Forward"].is_double() ? boost::json::value_to<double>(json_obj["Forward"]) : boost::json::value_to<long>(json_obj["Forward"]) ,
              json_obj["Alpha"].is_double() ? json_obj["Alpha"].as_double() : json_obj["Alpha"].as_int64() ,
              json_obj["Beta"].is_double() ? json_obj["Beta"].as_double() : json_obj["Beta"].as_int64() ,
              json_obj["Nu"].is_double() ? json_obj["Nu"].as_double() : json_obj["Nu"].as_int64() ,
              json_obj["Rho"].is_double() ? json_obj["Rho"].as_double() : json_obj["Rho"].as_int64() ,
              json_obj["AlphaIsFixed"].as_bool(),
              json_obj["BetaIsFixed"].as_bool(),
              json_obj["NuIsFixed"].as_bool(),
              json_obj["RhoIsFixed"].as_bool(),
              json_obj["VegaWeighted"].as_bool(),
              boost::json::value_to<std::string>(json_obj["EndCriteria"]),
              boost::json::value_to<std::string>(json_obj["OptimizationMeth"]),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}
