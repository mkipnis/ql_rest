#include <boost/test/included/unit_test.hpp>
#include <boost/assert.hpp>
using namespace boost::unit_test;

#include <qlrest/qlrest_common.h>
#include <boost/json/src.hpp>


void test_iso_date()
{
    auto date_serial_number = ql_rest::from_iso_string("2021-02-01");
    std::cout << "Serial Number : " << date_serial_number << std::endl;
    BOOST_TEST_REQUIRE( date_serial_number == 44228 );
    //BOOST_FAIL( "ISO Date conversion failed" );
}

void test_vector()
{
    auto json_vector = "[1.1,2.2,3.3,4.4,5.5,6.6]";
    auto request = boost::json::parse(json_vector).as_array();
    auto float_vector = ql_rest::vector_cast<float>(request);
    
    std::cout << float_vector[1] << std::endl;
}

void test_matrix()
{
    auto json_matrix = "[[1.1,2.2,3.3,4.4,5.5,6.6], " \
                       " [10.1,20.2,30.3,40.40,50.5,60.6], " \
                       " [100.1,200.2,300.3,400.40,500.5,600.6]]";
    
    /*boost::property_tree::ptree request;
    std::istringstream stream(json_matrix);
    boost::property_tree::read_json(stream, request);*/
    auto request = boost::json::parse(json_matrix);
    
    auto float_matrix = ql_rest::matrix_cast<float>(request);
}


void test_case4() { /* ... */ }

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
  test_suite* ts1 = BOOST_TEST_SUITE( "test_suite1" );
  ts1->add( BOOST_TEST_CASE( &test_iso_date ) );
  ts1->add( BOOST_TEST_CASE( &test_vector ) );
  ts1->add( BOOST_TEST_CASE( &test_matrix ) );

  test_suite* ts2 = BOOST_TEST_SUITE( "test_suite2" );
  //ts2->add( BOOST_TEST_CASE( &test_case3 ) );
  ts2->add( BOOST_TEST_CASE( &test_case4 ) );

  framework::master_test_suite().add( ts1 );
  framework::master_test_suite().add( ts2 );

  return 0;
}
