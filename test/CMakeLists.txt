##############################################################
# Copyright (c) 2008-2009 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

find_package(Firebird)
if(FIREBIRD_FOUND)
  include_directories(${Firebird_INCLUDE_DIRS})
  boost_test_run(firebird
    LINK_LIBS ${Firebird_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(FIREBIRD_FOUND)

find_package(MySQL)
if(MYSQL_FOUND)
  include_directories(${MYSQL_INCLUDE_DIR})
  boost_test_run(mysql
    LINK_LIBS ${MYSQL_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(MYSQL_FOUND)

find_package(ODBC)
if(ODBC_FOUND)
  include_directories(${ODBC_INCLUDE_DIRS})
  boost_test_run(odbc
    LINK_LIBS ${ODBC_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(ODBC_FOUND)

find_package(ORACLE)
if(ORACLE_FOUND)
  include_directories(${ORACLE_INCLUDE_DIRS})
  boost_test_run(oracle
    LINK_LIBS ${ORACLE_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(ORACLE_FOUND)

find_package(PostgreSQL)
if(POSTGRESQL_FOUND)
  include_directories(${PostgreSQL_INCLUDE_DIRS})
  boost_test_run(postgres
    LINK_LIBS ${PostgreSQL_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(POSTGRESQL_FOUND)

find_package(SQLite3)
if(SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
  boost_test_run(sqlite3
    LINK_LIBS ${SQLITE3_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(SQLITE3_FOUND)
