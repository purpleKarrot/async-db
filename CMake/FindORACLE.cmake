##############################################################
# Copyright (c) 2008 Daniel Pfeifer                          #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

# This module defines
# ORACLE_INCLUDE_DIRS - where to find oci.h
# ORACLE_LIBRARIES - the libraries to link against to use ORACLE
# ORACLE_FOUND - true if ORACLE was found

find_path(ORACLE_INCLUDE_DIRS oci.h
  /usr/include/oracle/*/client
)

find_library(ORACLE_LIBRARIES 
  NAMES
    clntsh
  PATHS
    /usr/lib/oracle/*/client/lib
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ORACLE
  DEFAULT_MSG ORACLE_LIBRARIES ORACLE_INCLUDE_DIRS)

mark_as_advanced(ORACLE_INCLUDE_DIRS ORACLE_LIBRARIES)
