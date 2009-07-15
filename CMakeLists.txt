##############################################################
# Copyright (c) 2008-2009 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
project(async-db)

set(CMAKE_BUILD_TYPE "Debug")
#add_definitions(-std=c++0x)

find_package(Boost 1.37.0 REQUIRED
  COMPONENTS filesystem system program_options)

##############################################################

set(BUILD_QUICKBOOK "ON")
add_executable(quickbook
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/quickbook.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/actions.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/actions_class.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/utils.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/post_process.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/collector.cpp
  ${CMAKE_SOURCE_DIR}/tools/quickbook/detail/template_stack.cpp
  )
target_link_libraries(quickbook ${Boost_LIBRARIES})

include(tools/BoostDocs.cmake)
include(tools/BoostUtils.cmake)

##############################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
find_package(Firebird)
find_package(MySQL)
find_package(ODBC)
find_package(ORACLE)
find_package(PostgreSQL)
find_package(SQLite3)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  )

add_subdirectory(doc)
add_subdirectory(examples)