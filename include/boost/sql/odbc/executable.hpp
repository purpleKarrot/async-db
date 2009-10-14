/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_EXECUTABLE_HPP
#define BOOST_SQL_ODBC_EXECUTABLE_HPP

#include <boost/sql/odbc/statement.hpp>
#include <boost/sql/detail/executable.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{

template<typename Signature>
struct executable: sql::detail::executable<connection, statement, Signature>
{
	typedef sql::detail::executable<connection, statement, Signature> base_type;

	executable(connection& conn, const std::string& query) :
		base_type(conn, query)
	{
	}
};

} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_EXECUTABLE_HPP */
