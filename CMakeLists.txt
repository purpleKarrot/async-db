##############################################################
# Copyright (c) 2008-2010 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project(async-db)

set(CMAKE_BUILD_TYPE "Debug")
#add_definitions(-std=c++0x)
add_definitions(-DBOOST_NO_SCOPED_ENUMS)
add_definitions(-DBOOST_NO_DEFAULTED_FUNCTIONS)

find_package(Boost 1.40.0 REQUIRED
  COMPONENTS filesystem system program_options date_time)

##############################################################

set(CTOOLBOX_USE_FILE ${CMAKE_BINARY_DIR}/UseCToolBox.cmake)
file(DOWNLOAD
  http://github.com/purpleKarrot/CToolBox/raw/master/UseCToolBox.cmake
  ${CTOOLBOX_USE_FILE})
include(${CTOOLBOX_USE_FILE})

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

option(SQL_BUILD_DOCUMENTATION OFF)
if(SQL_BUILD_DOCUMENTATION)
  set(THIS_PROJECT_NAME async-db)
  include(AddReference)
  include(AddDocumentation)
  add_subdirectory(doc)
endif(SQL_BUILD_DOCUMENTATION)

add_subdirectory(examples)
