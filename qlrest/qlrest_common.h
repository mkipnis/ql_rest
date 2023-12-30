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

#ifndef __common_h__
#define __common_h__

#include <ql/utilities/dataparsers.hpp>
#include <oh/objecthandler.hpp>

#include <iostream>

#include <boost/json.hpp>

#define OH_NULL ObjectHandler::property_t()

namespace ql_rest
{

inline long from_iso_string( const std::string& iso_date )
{
	return QuantLib::Date( QuantLib::DateParser::parseFormatted( iso_date, "%Y-%m-%d") ).serialNumber();
}


template<class QL_TYPE>
std::vector<QL_TYPE> vector_cast(  boost::json::array& data )
{
	std::vector<QL_TYPE> ql_data;

	for ( auto& json_value : data) 
	{
		ql_data.push_back( boost::json::value_to<QL_TYPE>(json_value) );
	}

	return ql_data;
}


template<class QL_TYPE>
std::vector<std::vector<QL_TYPE>> matrix_cast( const boost::json::value& data )
{
    std::vector<std::vector<QL_TYPE>> ql_data;

    for ( auto& x_value : data.as_array() )
    {
        ql_data.emplace_back( std::vector<QL_TYPE>() );
            
        for ( auto& y_value : x_value.as_array() )
        {
            ql_data.back().emplace_back( boost::json::value_to<QL_TYPE>(y_value) );
        }
    }

    return ql_data;
}


template<class QL_TYPE, class NATIVE_TYPE>
std::vector<std::vector<QL_TYPE>> matrix_cast( const boost::json::value& data  )
{
    std::vector<std::vector<QL_TYPE>> ql_data;

    for ( auto& x_value : data.as_array() )
    {
        ql_data.emplace_back( std::vector<QL_TYPE>() );
            
        for ( auto& y_value : x_value.as_array() )
        {
            ql_data.back().emplace_back(  boost::json::value_to<NATIVE_TYPE>(y_value) );
        }
    }

        return ql_data;
}



template<class QL_TYPE,class NATIVE_TYPE>
std::vector<QL_TYPE> vector_cast(  boost::json::array& data )
{
        std::vector<QL_TYPE> ql_data;

        for ( auto& json_value : data)
        {
                ql_data.push_back( QL_TYPE(boost::json::value_to<NATIVE_TYPE>(json_value)) );
        }

        return ql_data;
}


inline ObjectHandler::property_t from_iso_string_to_oh_property( const std::string& iso_date )
{
	if ( iso_date.empty() )
		return OH_NULL;

	return ObjectHandler::property_t( static_cast<long>( QuantLib::Date( QuantLib::DateParser::parseFormatted( iso_date, "%Y-%m-%d") ).serialNumber() ));
}

template <class QL_TYPE>
std::string from_ql_type_to_string( const QL_TYPE&& ql_type  )
{
    std::stringstream os;
    os << ql_type;
    return os.str();
};

template <class QL_TYPE>
std::string from_ql_type_to_string( const QL_TYPE& ql_type  )
{
    std::stringstream os;
    os << ql_type;
    return os.str();
};

};

#endif
