/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_STATEMENT_HPP
#define BOOST_SQL_ODBC_STATEMENT_HPP

#include <sql.h>

namespace boost
{
namespace sql
{
namespace odbc
{

template<typename Param, typename Result>
class statement
{
public:
	statement(connection& conn, const std::string& query) :
		query_str(query), prepared(false)
	{
		if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, conn.implementation(),
						&impl)))
		{
			throw std::bad_alloc();
		}

		if (!SQL_SUCCEEDED(SQLSetStmtAttr(impl, SQL_ATTR_CURSOR_TYPE,
						(SQLPOINTER) SQL_CURSOR_FORWARD_ONLY, SQL_IS_UINTEGER)))
		{
			detail::throw_error(SQL_HANDLE_STMT, impl);
		}
	}

	~statement()
	{
		SQLFreeHandle(SQL_HANDLE_STMT, impl);
	}

	void prepare()
	{
		if (!SQL_SUCCEEDED(SQLPrepare(impl, (SQLCHAR*)query_str.c_str(), query_str.length())))
			detail::throw_error(SQL_HANDLE_STMT, impl);

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();
	}

	bool fetch(Result&result)
	{
	}

private:
	HSTMT impl;
	std::string query_str;
	bool prepared;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_STATEMENT_HPP */
