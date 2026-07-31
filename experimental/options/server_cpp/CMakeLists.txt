cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0114 NEW)

project(options_pricer LANGUAGES CXX)

# =========================================================
# C++ STANDARD
# =========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(ExternalProject)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


# =========================================================
# BOOST
# =========================================================
find_package(Boost REQUIRED COMPONENTS filesystem regex json thread)

message(STATUS "Boost version: ${Boost_VERSION}")

# =========================================================
# HIREDIS
# =========================================================
find_path(HIREDIS_INCLUDE_DIR
    NAMES hiredis/hiredis.h
    PATHS
        /opt/homebrew/include
        /usr/local/include
        /usr/include
)

find_library(HIREDIS_LIBRARY
    NAMES hiredis
    PATHS
        /opt/homebrew/lib
        /usr/local/lib
        /usr/lib
        /usr/lib/x86_64-linux-gnu
)

if(NOT HIREDIS_INCLUDE_DIR OR NOT HIREDIS_LIBRARY)
    message(FATAL_ERROR "hiredis not found (install libhiredis-dev or brew install hiredis)")
endif()

message(STATUS "hiredis include: ${HIREDIS_INCLUDE_DIR}")
message(STATUS "hiredis lib: ${HIREDIS_LIBRARY}")

if(NOT DEFINED QUANTLIB_PREFIX)
    set(QUANTLIB_PREFIX
        ${CMAKE_BINARY_DIR}/_deps/quantlib_install
        CACHE PATH "QuantLib install prefix"
    )
endif()

ExternalProject_Add(quantlib_ep
    GIT_REPOSITORY https://github.com/lballabio/QuantLib.git

    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${QUANTLIB_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=ON
        -DQL_BUILD_TEST_SUITE=OFF
        -DQL_BUILD_EXAMPLES=OFF
        -DQL_INSTALL_TEST_SUITE=OFF
        -DQL_INSTALL_EXAMPLES=OFF
	-DQL_ENABLE_THREAD_SAFE_OBSERVER_PATTERN=ON
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_CXX_STANDARD=20

    INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)

# =========================================================
# EXECUTABLE
# =========================================================
add_executable(options_pricer options_pricer.cpp)

add_dependencies(options_pricer quantlib_ep)

target_include_directories(options_pricer PRIVATE
    ${HIREDIS_INCLUDE_DIR}
    ${QUANTLIB_PREFIX}/include
)

target_link_directories(options_pricer PRIVATE
    ${QUANTLIB_PREFIX}/lib
)

if(APPLE)
    set(QUANTLIB_LIBRARY
        ${QUANTLIB_PREFIX}/lib/libQuantLib.dylib
    )
elseif(UNIX)
    set(QUANTLIB_LIBRARY
        ${QUANTLIB_PREFIX}/lib/libQuantLib.so
    )
endif()

target_link_libraries(options_pricer PRIVATE
    ${HIREDIS_LIBRARY}
    Boost::filesystem
    Boost::regex
    Boost::json
    Boost::thread
    ${QUANTLIB_LIBRARY}
)

# =========================================================
# RPATH (macOS + Linux safe)
# =========================================================
if(APPLE)
    set_target_properties(options_pricer PROPERTIES
        BUILD_RPATH "${QUANTLIB_PREFIX}/lib"
        INSTALL_RPATH "${QUANTLIB_PREFIX}/lib"
        MACOSX_RPATH ON
    )
elseif(UNIX)
    set_target_properties(options_pricer PROPERTIES
        BUILD_RPATH "\$ORIGIN"
        INSTALL_RPATH "\$ORIGIN"
    )
endif()

# =========================================================
# DIAGNOSTICS
# =========================================================
message(STATUS "Build type       : ${CMAKE_BUILD_TYPE}")
message(STATUS "QuantLib prefix  : ${QUANTLIB_PREFIX}")
