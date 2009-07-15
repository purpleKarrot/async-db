/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_EXECUTABLE_HPP
#define BOOST_SQL_MYSQL_EXECUTABLE_HPP

#include <boost/sql/mysql/statement.hpp>
#include <boost/sql/detail/executable.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

#if(0)

// requires C++0x Template Typedefs
template<typename Signature>
using executable = sql::detail::executable<connection, statement, Signature>;

#else

template<typename Signature>
struct executable: sql::detail::executable<connection, statement, Signature>
{
	typedef sql::detail::executable<mysql::connection, mysql::statement,
			Signature> base_type;

	executable(connection& conn, const std::string& query) :
		base_type(conn, query)
	{
	}
};

#endif

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_EXECUTABLE_HPP */
