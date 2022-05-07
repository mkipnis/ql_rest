
TestSuite_CPPFLAGS = -I${deps_dir_include} -I${deps_dir_include}/qlo -I../
TestSuite_LDADD = ../qlrest/libQLRest.la
TestSuite_LDFLAGS = -lObjectHandler -lQuantLib -lQuantLibAddin -lpthread -lQuantLibAddinCpp -lboost_filesystem -lboost_serialization -lboost_system -lboost_regex -lboost_date_time

TestSuite_SOURCES = common_test.cpp

noinst_PROGRAMS = TestSuite
