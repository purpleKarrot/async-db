/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_SQLITE3_STATEMENT_HPP
#define BOOST_SQL_SQLITE3_STATEMENT_HPP

#include <boost/sql/sqlite3/connection.hpp>
#include <boost/sql/sqlite3/detail/bind_params.hpp>
#include <boost/sql/sqlite3/detail/bind_result.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/sql/detail/executable.hpp>
#include <iostream>
#include <string>

namespace boost
{
namespace sql
{
namespace sqlite3
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
	statement(connection& c, const std::string& query) :
		stmt(0), conn(c), query_str(query), prepared(false)
	{
	}

	~statement()
	{
		sqlite3_finalize(stmt);
	}

	void prepare()
	{
		if (sqlite3_prepare_v2(conn.implementation(), query_str.c_str(),
				query_str.size(), &stmt, 0) != SQLITE_OK)
		{
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
		}

		BOOST_ASSERT(sqlite3_bind_parameter_count(stmt) == param_count);
		BOOST_ASSERT(sqlite3_column_count(stmt) == result_count);

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		if (sqlite3_reset(stmt) != SQLITE_OK)
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));

		if (detail::bind_params(stmt, params))
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));

		int (*fn)(sqlite3_stmt*) = sqlite3_column_count(stmt) ?
		sqlite3_reset : sqlite3_step;
		int rc = fn(stmt);
		if (rc != SQLITE_DONE && rc != SQLITE_OK)
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
	}

	bool fetch(Result& result)
	{
		switch (sqlite3_step(stmt))
		{
		case SQLITE_ROW:
			fusion::for_each(result, detail::bind_result(stmt));
			return true;
		case SQLITE_DONE:
			sqlite3_reset(stmt);
			return false;
		case SQLITE_ERROR:
			// SQLITE_ERROR is a generic error code.
			// must call sqlite3_reset() to get the specific error message.
			sqlite3_reset(stmt);
		}

		throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
	}

private:
	sqlite3_stmt* stmt;
	connection& conn;
	std::string query_str;
	bool prepared;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_STATEMENT_HPP*/
