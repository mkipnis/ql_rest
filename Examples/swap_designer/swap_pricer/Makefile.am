
SwapPricer_CPPFLAGS = -I${deps_dir_include} -I${deps_dir_include}/qlo -I../../../
SwapPricer_LDADD = ../../../qlrest/libQLRest.la
SwapPricer_LDFLAGS = -lObjectHandler -lQuantLib -lQuantLibAddin -lpthread -lQuantLibAddinCpp -lboost_filesystem -lboost_serialization -lboost_system -lboost_regex -lboost_date_time

SwapPricer_SOURCES = main.cpp \
		pricing_thread.cpp \
		swap_pricer_listener.cpp \	
		swap_pricer_session.cpp

noinst_PROGRAMS = SwapPricer
