
# Process this file with autoconf to produce a configure script.

AC_INIT([QLRest], [0.0.1],
        [mike.kipnis@gmail.com],
        [QLRest])
AC_PREREQ(2.50)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([qlrest/config.hpp])
AM_INIT_AUTOMAKE([foreign])

AC_COPYRIGHT([mike kipnis copyright])

AC_PROG_INSTALL

# Default compilation variables

AC_SUBST([CXXFLAGS],[${CXXFLAGS='-g -O2 -Wall -std=c++0x'}])

# Continue setup

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_LANG([C++])

# Configure and validate the path to Boost/QuantLib/ObjectHandler/QuantLibAddin 

AC_ARG_WITH([deps-include], AC_HELP_STRING([--with-deps-include=/path/to/quantlib], [Specify the path to dependances boost/beast, ql/quantlib, oh/object_handler, qlo/quantlibaddin]), [deps_path_include=${withval}], [AC_MSG_ERROR([You must specify the location of dependances: "./configure --with-deps-include=path/to/dependances/include"])])
AC_CHECK_FILES([${deps_path_include}/boost/beast.hpp ${deps_path_include}/ql/version.hpp ${deps_path_include}/oh/objecthandler.hpp ${deps_path_include}/qlo/qladdin.hpp], [], [AC_MSG_ERROR([Unable to locate beast.hpp ql/version.hpp oh/objecthandler.hpp qlo/qladdin.hpp in specified dependances directory])])
AC_ARG_VAR(deps_dir_include, "path to dependances include dir")
AC_SUBST(deps_dir_include, [${deps_path_include}])

AC_ARG_WITH([deps-lib], AC_HELP_STRING([--with-deps-lib=LIB_PATH], 
	[Specify the path to dependances libraries boost,quantlib,object_handler,quantlibadd]), [deps_path_lib="`cd ${withval} 2>/dev/null && pwd`"], [deps_path_lib=""])
AC_SUBST([DEPS_LIB],["-L${deps_path_lib}"])
AC_SUBST([LDFLAGS],["${LDFLAGS} -L${deps_path_lib}"])


# Done, output the configured files

AC_CONFIG_FILES([
    Makefile
    qlrest/Makefile
    test-suite/Makefile
    Examples/bond_pricer/bond_pricer_server/Makefile])

AC_OUTPUT

