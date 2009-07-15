##############################################################
# Copyright (c) 2008 Daniel Pfeifer                          #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

# This module defines
# PostgreSQL_INCLUDE_DIRS - where to find libpq-fe.h
# PostgreSQL_LIBRARIES - the libraries to link against to use PostgreSQL
# PostgreSQL_FOUND - true if PostgreSQL was found

find_path(PostgreSQL_INCLUDE_DIRS libpq-fe.h
  /usr/include
  /usr/include/postgresql
  $ENV{ProgramFiles}/PostgreSQL/*/include
)

find_library(PostgreSQL_LIBRARIES
  NAMES
    pq
    libpq
  PATHS
    /usr/lib
    $ENV{ProgramFiles}/PostgreSQL/*/lib
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PostgreSQL
  DEFAULT_MSG PostgreSQL_LIBRARIES PostgreSQL_INCLUDE_DIRS)

mark_as_advanced(PostgreSQL_INCLUDE_DIRS PostgreSQL_LIBRARIES)
