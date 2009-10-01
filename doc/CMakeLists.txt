##############################################################
# Copyright (c) 2008-2009 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

boost_add_documentation(sql.qbk
  HEADERS
    sql_autodoc.xml

    boost/sql.hpp
    boost/sql/mysql.hpp
    boost/sql/odbc.hpp
    boost/sql/oracle.hpp
    boost/sql/postgres.hpp
    boost/sql/sqlite3.hpp

    boost/sql/firebird/connection.hpp
    boost/sql/firebird/statement.hpp

    boost/sql/mysql/connection.hpp
    boost/sql/mysql/statement.hpp

    boost/sql/postgres/connection.hpp
    boost/sql/postgres/statement.hpp

    boost/sql/odbc/connection.hpp
    boost/sql/odbc/statement.hpp

    boost/sql/sqlite3/connection.hpp
    boost/sql/sqlite3/statement.hpp

  DOXYGEN_PARAMETERS
    EXTRACT_ALL=YES
    MACRO_EXPANSION=YES
    SORT_BY_SCOPE_NAME=YES
  )

configure_file(boostbook.css
  ${CMAKE_CURRENT_BINARY_DIR}/html/boostbook.css COPYONLY)

configure_file(exploits_of_a_mom.png
  ${CMAKE_CURRENT_BINARY_DIR}/html/exploits_of_a_mom.png COPYONLY)

install(FILES
    boostbook.css
    exploits_of_a_mom.png
  DESTINATION
    share/boost-${BOOST_VERSION}/html
  )
