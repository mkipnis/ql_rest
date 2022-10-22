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

/* Autogenerated from the following stub : git/ql_rest/gensrc/stubs/stub.qlrest.header */

#include <qlo/qladdindefines.hpp>
#include <qlo/handle.hpp>
#include <qlo/piecewiseyieldcurve.hpp>
#include <qlo/ratehelpers.hpp>
#include <qlo/handleimpl.hpp>
#include <ql/interestrate.hpp>
#include <qlo/conversions/coercetermstructure.hpp>
#include <qlo/valueobjects/vo_piecewiseyieldcurve.hpp>

#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>

#include "qlrest_common.h"

#include <qlo/Addins/Cpp/addincpp.hpp>

namespace ql_rest
{
    struct piecewiseyieldcurve
    {
      
        static std::string qlPiecewiseYieldCurve(ptree const& pt);  
        static std::string qlPiecewiseYieldCurveMixedInterpolation(ptree const& pt);
    };
};
