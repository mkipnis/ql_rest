cmake_minimum_required(VERSION 3.15)

project(ql_rest_lib)

file(GLOB QL_REST_LIB_SRC "*.cpp")
file(GLOB QL_REST_LIB_HDR "*.hpp")

add_library(ql_rest_lib SHARED ${QL_REST_LIB_SRC})

target_include_directories(ql_rest_lib PRIVATE ${QuantLibAddin_INCLUDE_DIRS})
target_link_directories(ql_rest_lib PRIVATE ${QuantLibAddin_LIBRARY_DIRS})
target_link_libraries(ql_rest_lib PRIVATE Boost::json Boost::filesystem ${QuantLibAddin_LIBS})

install(TARGETS ql_rest_lib
        EXPORT ql_restTargets
        DESTINATION lib)

install(FILES ${QL_REST_LIB_HDR}
        DESTINATION include/qlrest)

install(EXPORT ql_restTargets
        FILE ql_restTargets.cmake
        NAMESPACE ql_rest::
        DESTINATION lib/cmake/ql_rest)
