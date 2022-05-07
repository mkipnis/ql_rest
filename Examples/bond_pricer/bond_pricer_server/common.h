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

#ifndef common_h
#define common_h

#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/lockfree/queue.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

/*
#include <Addins/Cpp/ratehelpers.hpp>
#include <Addins/Cpp/pricingengines.hpp>
#include <Addins/Cpp/piecewiseyieldcurve.hpp>
#include <qlo/piecewiseyieldcurve.hpp>
#include <qlo/bonds.hpp>

#include <oh/ohdefines.hpp>
#include <oh/property.hpp>
#include <oh/repository.hpp>

#include <Addins/Cpp/addincpp.hpp>
#include <ql/quantlib.hpp>
*/

#include <ql/quantlib.hpp>
#include <Addins/Cpp/addincpp.hpp>
#include <oh/ohdefines.hpp>

#include <qlrest/qlrest_common.h>

using json_raw_ptr = boost::property_tree::ptree*;
using json_ptr_pc_queue = boost::lockfree::queue< json_raw_ptr >;
using json_ptr_pc_queue_ptr = std::shared_ptr<json_ptr_pc_queue>;

#endif /* common_h */
