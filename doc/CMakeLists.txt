##############################################################
# Copyright (c) 2008-2010 Daniel Pfeifer                     #
#                                                            #
# Distributed under the Boost Software License, Version 1.0. #
##############################################################

add_reference(sql-ref.xml

  ../include/boost/sql.hpp

  ../include/boost/sql/mysql.hpp
  ../include/boost/sql/odbc.hpp
  ../include/boost/sql/oracle.hpp
  ../include/boost/sql/postgres.hpp
  ../include/boost/sql/sqlite3.hpp

  ../include/boost/sql/firebird/connection.hpp
  ../include/boost/sql/firebird/statement.hpp

  ../include/boost/sql/mysql/connection.hpp
  ../include/boost/sql/mysql/statement.hpp

  ../include/boost/sql/postgres/connection.hpp
  ../include/boost/sql/postgres/statement.hpp

  ../include/boost/sql/odbc/connection.hpp
  ../include/boost/sql/odbc/statement.hpp

  ../include/boost/sql/sqlite3/connection.hpp
  ../include/boost/sql/sqlite3/statement.hpp
  )

add_documentation(
  sql.qbk
  sql-ref.xml
  )
