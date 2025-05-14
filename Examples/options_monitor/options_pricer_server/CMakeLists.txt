

file(GLOB QL_REST_LIB_SRC "*.cpp")

add_executable(OptionsPricer ${QL_REST_LIB_SRC})

target_include_directories(OptionsPricer
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${QuantLib_INCLUDE_DIRS}
        ${QuantLibAddin_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}
)

target_link_directories(OptionsPricer PRIVATE ${QuantLibAddin_LIBRARY_DIRS})
target_link_libraries(OptionsPricer PRIVATE Boost::json Boost::filesystem ${QuantLibAddin_LIBS} ql_rest_lib)

set_target_properties(OptionsPricer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
