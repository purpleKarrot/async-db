##############################################################
# Copyright (c) 2008 Daniel Pfeifer                          #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

# This module defines
# SQLITE3_INCLUDE_DIRS - where to find sqlite3.h
# SQLITE3_LIBRARIES - the libraries to link against to use SQLite3
# SQLITE3_FOUND - true if SQLite3 was found

find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(SQLITE3 sqlite3)
endif(PKG_CONFIG_FOUND)

mark_as_advanced(SQLITE3_INCLUDE_DIRS SQLITE3_LIBRARIES)
