
OptionsPricer_CPPFLAGS = -I${deps_dir_include} -I${deps_dir_include}/qlo -I../../../
OptionsPricer_LDADD = ../../../qlrest/libQLRest.la
OptionsPricer_LDFLAGS = -lObjectHandler -lQuantLib -lQuantLibAddin -lpthread -lQuantLibAddinCpp -lboost_filesystem -lboost_serialization -lboost_system -lboost_regex -lboost_date_time

OptionsPricer_SOURCES = options_pricer.cpp


noinst_PROGRAMS = OptionsPricer
