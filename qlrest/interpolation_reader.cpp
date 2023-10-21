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



std::string interpolation::qlAbcdInterpolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlAbcdInterpolation(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::vector_cast<double>(pt.get_child("XArray")),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(pt.get_child("YArray")),
              pt.get<double>("A"),
              pt.get<double>("B"),
              pt.get<double>("C"),
              pt.get<double>("D"),
              pt.get<bool>("AIsFixed"),
              pt.get<bool>("BIsFixed"),
              pt.get<bool>("CIsFixed"),
              pt.get<bool>("DIsFixed"),
              pt.get<bool>("VegaWeighted"),
              pt.get<std::string>("EndCriteria"),
              pt.get<std::string>("OptimizationMeth"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string interpolation::qlCubicInterpolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlCubicInterpolation(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::vector_cast<double>(pt.get_child("XArray")),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(pt.get_child("YArray")),
              pt.get<std::string>("DerApprox"),
              pt.get<bool>("Monotonic"),
              pt.get<std::string>("LeftConditionType"),
              pt.get<double>("LeftConditionValue"),
              pt.get<std::string>("RightConditionType"),
              pt.get<double>("RightConditionValue"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

bool interpolation::qlExtrapolatorEnableExtrapolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlExtrapolatorEnableExtrapolation(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("ExtrapolationFlag"),
              pt.get<bool>("Trigger")
    );
}

std::string interpolation::qlInterpolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlInterpolation(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("InterpolationType"),
              ql_rest::vector_cast<double>(pt.get_child("XArray")),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(pt.get_child("YArray")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string interpolation::qlMixedLinearCubicInterpolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlMixedLinearCubicInterpolation(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::vector_cast<double>(pt.get_child("XArray")),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(pt.get_child("YArray")),
              pt.get<long>("SwitchIndex"),
              pt.get<std::string>("MixedInterpolationBehavior"),
              pt.get<std::string>("DerApprox"),
              pt.get<bool>("Monotonic"),
              pt.get<std::string>("LeftConditionType"),
              pt.get<double>("LeftConditionValue"),
              pt.get<std::string>("RightConditionType"),
              pt.get<double>("RightConditionValue"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string interpolation::qlSABRInterpolation(ptree const& pt)
{
    return QuantLibAddinCpp::qlSABRInterpolation(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::vector_cast<double>(pt.get_child("XArray")),
             ql_rest::vector_cast<ObjectHandler::property_t,std::string>(pt.get_child("YArray")),
              pt.get<double>("Expiry"),
              ObjectHandler::property_t(pt.get<std::string>("Forward")) ,
              pt.get<double>("Alpha"),
              pt.get<double>("Beta"),
              pt.get<double>("Nu"),
              pt.get<double>("Rho"),
              pt.get<bool>("AlphaIsFixed"),
              pt.get<bool>("BetaIsFixed"),
              pt.get<bool>("NuIsFixed"),
              pt.get<bool>("RhoIsFixed"),
              pt.get<bool>("VegaWeighted"),
              pt.get<std::string>("EndCriteria"),
              pt.get<std::string>("OptimizationMeth"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}