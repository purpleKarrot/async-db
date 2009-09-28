/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_STATEMENT_HPP
#define BOOST_SQL_ODBC_STATEMENT_HPP

#include <sql.h>
#include <boost/sql/odbc/detail/bind_param.hpp>
#include <boost/sql/odbc/detail/bind_result.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl/size.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{

template<typename Param, typename Result>
class statement
{
	enum
	{
		param_count = mpl::size<Param>::value,
		result_count = mpl::size<Result>::value
	};

public:
	statement(connection& conn, const std::string& query) :
		conn(conn), impl(0), query_str(query), prepared(false)
	{
	}

	~statement()
	{
		SQLFreeHandle(SQL_HANDLE_STMT, impl);
	}

	void prepare()
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

		if (!SQL_SUCCEEDED(SQLPrepare(impl, (SQLCHAR*)query_str.c_str(), query_str.length())))
		{
			detail::throw_error(SQL_HANDLE_STMT, impl);
		}

		prepared = true;
	}

	void execute(const Param& param)
	{
		if (!prepared)
			prepare();

		SQLINTEGER indicators[param_count];
		fusion::for_each(param, detail::bind_param(impl, indicators));

		if (!SQL_SUCCEEDED(SQLExecute(impl)))
			detail::throw_error(SQL_HANDLE_STMT, impl);
	}

	bool fetch(Result& result)
	{
		SQLCHAR SalesPerson[11];
		SQLUINTEGER CustID;
		SQLINTEGER SalesPersonLenOrInd, CustIDInd;

		SQLINTEGER indicators[result_count];
		fusion::for_each(result, detail::bind_result(impl, indicators));

		SQLRETURN rc = SQLFetch(impl);

		if (rc == SQL_NO_DATA)
			return false;

		if (!SQL_SUCCEEDED(rc))
			detail::throw_error(SQL_HANDLE_STMT, impl);

		return true;
	}

private:

	connection& conn;

	HSTMT impl;
	std::string query_str;
	bool prepared;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_STATEMENT_HPP */
