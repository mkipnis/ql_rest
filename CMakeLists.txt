cmake_minimum_required(VERSION 3.15)

project(ql_rest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED COMPONENTS filesystem regex json)

set(QLA_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/qladdin)

include(ExternalProject)

find_package(QuantLibAddin REQUIRED)

add_subdirectory(qlrest)
add_subdirectory(Examples/bond_pricer/bond_pricer_server)
add_subdirectory(Examples/swap_designer/swap_pricer)
add_subdirectory(Examples/options_monitor/options_pricer_server)
