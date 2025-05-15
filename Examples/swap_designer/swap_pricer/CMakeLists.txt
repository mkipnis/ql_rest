

file(GLOB QL_REST_LIB_SRC "*.cpp")

add_executable(SwapPricer ${QL_REST_LIB_SRC})

target_include_directories(SwapPricer
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${QuantLib_INCLUDE_DIRS}
        ${QuantLibAddin_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}
)

target_link_directories(SwapPricer PRIVATE ${QuantLibAddin_LIBRARY_DIRS})
target_link_libraries(SwapPricer PRIVATE Boost::json Boost::filesystem ${QuantLibAddin_LIBS} ql_rest_lib)

set_target_properties(SwapPricer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
