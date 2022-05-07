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

#include "schedule_reader.hpp"

using namespace ql_rest;



std::string schedule::qlSchedule(ptree const& pt)
{
    return QuantLibAddinCpp::qlSchedule(
         
              pt.get<std::string>("ObjectId"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("EffectiveDate")),
              ql_rest::from_iso_string(pt.get<std::string>("TerminationDate") ),
              pt.get<std::string>("Tenor"),
              pt.get<std::string>("Calendar"),
              pt.get<std::string>("Convention"),
              pt.get<std::string>("TermDateConv"),
              pt.get<std::string>("GenRule"),
              pt.get<bool>("EndOfMonth"),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("FirstDate")),
              ql_rest::from_iso_string_to_oh_property(pt.get<std::string>("NextToLastDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string schedule::qlScheduleFromDateVector(ptree const& pt)
{
    return QuantLibAddinCpp::qlScheduleFromDateVector(
         
              pt.get<std::string>("ObjectId"),
             ql_rest::vector_cast<ObjectHandler::property_t,long>(pt.get_child("EffectiveDate")),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}

std::string schedule::qlScheduleTruncated(ptree const& pt)
{
    return QuantLibAddinCpp::qlScheduleTruncated(
         
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("OriginalSchedule"),
              ql_rest::from_iso_string(pt.get<std::string>("TruncationDate") ),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}
