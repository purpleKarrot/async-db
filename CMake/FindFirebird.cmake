##############################################################
# Copyright (c) 2008 Daniel Pfeifer                          #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

# This module defines
# Firebird_INCLUDE_DIRS - where to find ibase.h
# Firebird_LIBRARIES - the libraries to link against to use Firebird
# Firebird_FOUND - true if Firebird was found

find_path(Firebird_INCLUDE_DIRS ibase.h
  /usr/include
  $ENV{ProgramFiles}/Firebird/*/include
)

find_library(Firebird_LIBRARIES
  NAMES
    fbclient
    fbclient_ms
  PATHS
    /usr/lib
    $ENV{ProgramFiles}/Firebird/*/lib
)

# fbembed ?

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Firebird
  DEFAULT_MSG Firebird_LIBRARIES Firebird_INCLUDE_DIRS)

mark_as_advanced(Firebird_INCLUDE_DIRS Firebird_LIBRARIES)
