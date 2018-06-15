cmake_minimum_required(VERSION 3.2)
project(ompparser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE Debug)

set(OMPPARSER_VERSION_MAJOR 0)
set(OMPPARSER_VERSION_MINOR 1)
set(OMPPARSER_VERSION ${OMPPARSER_VERSION_MAJOR}.${OMPPARSER_VERSION_MINOR})

find_package(BISON)
find_package(FLEX)

configure_file(src/ompparser_config.h.cmake "ompparser_config.h" @ONLY)

# Not used, but keep this for the purpose to keep track of the original source files
set(OMPPARSER_SOURCE_FILES
    src/omplexer.ll
    src/ompparser.yy
    src/OpenMPAttribute.h
    src/OpenMPAttribute.cpp)

BISON_TARGET(OMPBISONParser src/ompparser.yy ${CMAKE_CURRENT_BINARY_DIR}/ompparser.yy.cpp)
FLEX_TARGET(OMPFLEXScanner src/omplexer.ll ${CMAKE_CURRENT_BINARY_DIR}/omplexer.ll.cpp)
ADD_FLEX_BISON_DEPENDENCY(OMPFLEXScanner OMPBISONParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(ompparser SHARED ${BISON_OMPBISONParser_OUTPUTS} ${FLEX_OMPFLEXScanner_OUTPUTS} src/OpenMPAttribute.cpp)

set(ompparser_targets ompparser)

# Install headers and libraries
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ompparser_config.h
        OpenMPAttribute.h
        DESTINATION include)
install(TARGETS ${ompparser_targets}
        LIBRARY DESTINATION lib
        )
