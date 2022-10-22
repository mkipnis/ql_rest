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

#include "garbagecollection_reader.hpp"

using namespace ql_rest;



bool garbagecollection::ohRepositoryDeleteAllObjects(ptree const& pt)
{
    return QuantLibAddinCpp::ohRepositoryDeleteAllObjects(
         
              pt.get<bool>("DeletePermanent"),
              pt.get<bool>("Trigger")
    );
}

bool garbagecollection::ohRepositoryLogObject(ptree const& pt)
{
    return QuantLibAddinCpp::ohRepositoryLogObject(
         
              pt.get<std::string>("ObjectId"),
              pt.get<bool>("Trigger")
    );
}
