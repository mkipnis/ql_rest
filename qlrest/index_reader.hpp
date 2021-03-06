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

/* Autogenerated from the following stub : ql_rest/gensrc/stubs/stub.qlrest.header */

#include <qlo/qladdindefines.hpp>
#include <qlo/handle.hpp>
#include <qlo/indexes/bmaindex.hpp>
#include <qlo/indexes/ibor/euribor.hpp>
#include <qlo/indexes/ibor/libor.hpp>
#include <qlo/indexes/swap/euriborswap.hpp>
#include <qlo/indexes/swap/liborswap.hpp>
#include <qlo/indexes/swap/isdafixaswap.hpp>
#include <qlo/termstructures.hpp>
#include <qlo/handleimpl.hpp>
#include <qlo/timeseries.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/indexes/iborindex.hpp>
#include <ql/experimental/coupons/proxyibor.hpp>
#include <ql/indexes/swapindex.hpp>
#include <qlo/valueobjects/vo_index.hpp>

#include <qlo/conversions/all.hpp>
#include <oh/property.hpp>

#include "qlrest_common.h"

#include <qlo/Addins/Cpp/addincpp.hpp>

namespace ql_rest
{
    struct index
    {
      
        static std::string qlBMAIndex(ptree const& pt);  
        static std::string qlEonia(ptree const& pt);  
        static std::string qlEuribor(ptree const& pt);  
        static std::string qlEuribor365(ptree const& pt);  
        static std::string qlEuriborSwap(ptree const& pt);  
        static std::string qlEuriborSwapIsdaFixA(ptree const& pt);  
        static std::string qlIborIndex(ptree const& pt);  
        static bool qlIndexAddFixings(ptree const& pt);  
        static std::string qlLibor(ptree const& pt);  
        static std::string qlLiborSwap(ptree const& pt);  
        static std::string qlOvernightIndex(ptree const& pt);  
        static std::string qlProxyIbor(ptree const& pt);  
        static std::string qlSonia(ptree const& pt);  
        static std::string qlSwapIndex(ptree const& pt);
    };
};
