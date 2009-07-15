##############################################################
# Copyright (c) 2008 Daniel Pfeifer                          #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

# This module defines
# ODBC_INCLUDE_DIRS - where to find sql.h
# ODBC_LIBRARIES - the libraries to link against to use ODBC
# ODBC_FOUND - true if ODBC was found

find_path(ODBC_INCLUDE_DIRS sql.h
  /usr/include
  "$ENV{ProgramFiles}/Microsoft SDKs/Windows/*/Include"
)
  
find_library(ODBC_LIBRARIES 
  NAMES
    iodbc
    odbc
	odbc32
  PATHS
    /usr/lib
    "$ENV{ProgramFiles}/Microsoft SDKs/Windows/*/Lib"
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ODBC
  DEFAULT_MSG ODBC_LIBRARIES ODBC_INCLUDE_DIRS)

mark_as_advanced(ODBC_INCLUDE_DIRS ODBC_LIBRARIES)
