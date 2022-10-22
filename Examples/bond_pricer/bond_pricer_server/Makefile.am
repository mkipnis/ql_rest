
BondPricer_CPPFLAGS = -I${deps_dir_include} -I${deps_dir_include}/qlo -I../../../
BondPricer_LDADD = ../../../qlrest/libQLRest.la
BondPricer_LDFLAGS = -lObjectHandler -lQuantLib -lQuantLibAddin -lpthread -lQuantLibAddinCpp -lboost_filesystem -lboost_serialization -lboost_system -lboost_regex -lboost_date_time

BondPricer_SOURCES = main.cpp curve_builder.cpp bond_pricer.cpp


noinst_PROGRAMS = BondPricer
