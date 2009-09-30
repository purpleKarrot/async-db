##############################################################
# Copyright (c) 2008-2009 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

if(FIREBIRD_FOUND)
  include_directories(${Firebird_INCLUDE_DIRS})
  add_executable(example-firebird firebird.cpp)
  target_link_libraries(example-firebird
    ${Firebird_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(FIREBIRD_FOUND)

if(MYSQL_FOUND)
  include_directories(${MYSQL_INCLUDE_DIR})
  add_executable(example-mysql mysql.cpp)
  target_link_libraries(example-mysql
    ${MYSQL_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
endif(MYSQL_FOUND)

if(ODBC_FOUND)
  include_directories(${ODBC_INCLUDE_DIRS})
  add_executable(example-odbc odbc.cpp)
  target_link_libraries(example-odbc
    ${ODBC_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(ODBC_FOUND)

if(ORACLE_FOUND)
  include_directories(${ORACLE_INCLUDE_DIRS})
  add_executable(example-oracle oracle.cpp)
  target_link_libraries(example-oracle
    ${ORACLE_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(ORACLE_FOUND)

if(POSTGRESQL_FOUND)
  include_directories(${PostgreSQL_INCLUDE_DIRS})
  add_executable(example-postgres postgres.cpp)
  target_link_libraries(example-postgres
    ${PostgreSQL_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(POSTGRESQL_FOUND)

if(SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
  add_executable(example-sqlite3 sqlite3.cpp)
  target_link_libraries(example-sqlite3
    ${SQLITE3_LIBRARIES} ${Boost_SYSTEM_LIBRARY})
endif(SQLITE3_FOUND)
