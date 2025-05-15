#if(NOT DEFINED QLA_ROOT_DIR)
#    # Fall back to location relative to this file if not passed explicitly
#    get_filename_component(QLA_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
#    get_filename_component(QLA_ROOT_DIR "${QLA_CMAKE_DIR}/../.." ABSOLUTE)
#endif()

set(QuantLibAddin_INCLUDE_DIRS "${QLA_ROOT_DIR}/include")
set(QuantLibAddin_LIBRARY_DIRS "${QLA_ROOT_DIR}/lib")

include(ExternalProject)

if(NOT EXISTS "${QuantLibAddin_INCLUDE_DIRS}/qlo/config.hpp")
    message(STATUS "QuantLibAddin not found, building from source...")

    set(QLA_ROOT_DIR ${CMAKE_BINARY_DIR}/qladdin)

    include(ExternalProject)
    ExternalProject_Add(QuantLibAddinOld
        GIT_REPOSITORY https://github.com/mkipnis/QuantLibAddin-Old.git
        GIT_TAG cmake
        UPDATE_DISCONNECTED TRUE
        INSTALL_DIR ""
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${QLA_INSTALL_PREFIX}
    )

    list(APPEND CMAKE_PREFIX_PATH ${QLA_ROOT_DIR})
    set(QuantLibAddin_LIBRARY_DIRS "${QLA_ROOT_DIR}/lib")

endif()

list(APPEND QuantLibAddin_INCLUDE_DIRS "${QLA_ROOT_DIR}/include")
list(APPEND QuantLibAddin_INCLUDE_DIRS "${QLA_ROOT_DIR}/include/qlo")

set(QuantLibAddin_LIBRARIES QuantLibAddin ObjectHandler QuantLib)

set(QuantLibAddin_FOUND TRUE)
set(QuantLibAddin_LIBS QuantLibAddin ObjectHandler QuantLib QuantLibAddinCpp)
