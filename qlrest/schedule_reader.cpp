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

#include "schedule_reader.hpp"

using namespace ql_rest;



std::string schedule::qlSchedule(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlSchedule(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["EffectiveDate"])),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["TerminationDate"])),
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              boost::json::value_to<std::string>(json_obj["TermDateConv"]),
              boost::json::value_to<std::string>(json_obj["GenRule"]),
              json_obj["EndOfMonth"].as_bool(),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["FirstDate"])),
              ql_rest::from_iso_string_to_oh_property(boost::json::value_to<std::string>(json_obj["NextToLastDate"])),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string schedule::qlScheduleFromDateVector(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlScheduleFromDateVector(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
             ql_rest::vector_cast<ObjectHandler::property_t,long>(json_obj["EffectiveDate"].as_array()),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string schedule::qlScheduleFullInterfaceFromDateVector(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlScheduleFullInterfaceFromDateVector(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
             ql_rest::vector_cast<ObjectHandler::property_t,long>(json_obj["Dates"].as_array()),
              ql_rest::vector_cast<bool>(json_obj["IsRegular"].as_array()),
              boost::json::value_to<std::string>(json_obj["Tenor"]),
              boost::json::value_to<std::string>(json_obj["Calendar"]),
              boost::json::value_to<std::string>(json_obj["Convention"]),
              boost::json::value_to<std::string>(json_obj["TermDateConv"]),
              boost::json::value_to<std::string>(json_obj["GenRule"]),
              json_obj["EndOfMonth"].as_bool(),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}

std::string schedule::qlScheduleTruncated(boost::json::object& json_obj)
{
    return QuantLibAddinCpp::qlScheduleTruncated(
         
              boost::json::value_to<std::string>(json_obj["ObjectId"]),
              boost::json::value_to<std::string>(json_obj["OriginalSchedule"]),
              ql_rest::from_iso_string(boost::json::value_to<std::string>(json_obj["TruncationDate"])),
              json_obj["ReturnDatesBefore"].as_bool(),
              json_obj["Permanent"].as_bool(),
              json_obj["Trigger"].as_bool(),
              json_obj["Overwrite"].as_bool()
    );
}
