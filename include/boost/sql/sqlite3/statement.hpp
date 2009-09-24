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
#include <boost/sql/sqlite3/detail/error.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/sql/detail/executable.hpp>
#include <boost/sql/detail/handler.hpp>
#include <boost/bind.hpp>
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

	std::string error_message() const
	{
		return sqlite3_errmsg(conn.implementation());
	}

	void prepare(boost::system::error_code& error)
	{
		error.assign(sqlite3_prepare_v2(conn.implementation(),
				query_str.c_str(), query_str.size(), &stmt, 0),
				get_error_category());

		if (!error)
		{
			BOOST_ASSERT(sqlite3_bind_parameter_count(stmt) == param_count);
			BOOST_ASSERT(sqlite3_column_count(stmt) == result_count);

			prepared = true;
		}
	}

	void prepare()
	{
		boost::system::error_code error;
		prepare(error);
		if (error)
			throw std::runtime_error(error_message());
	}

	template<typename Handler>
	void async_prepare(Handler handler)
	{
		post(boost::bind(statement::prepare, this, _1), handler);
	}

	void execute(const Param& params, boost::system::error_code& error)
	{
		if (!prepared)
		{
			prepare(error);
			if (error)
				return;
		}

		error.assign(sqlite3_reset(stmt), get_error_category());
		if (error)
			return;

		error.assign(detail::bind_params(stmt, params), get_error_category());
		if (error)
			return;

		typedef int(*step_or_reset_function)(sqlite3_stmt*);
		step_or_reset_function step_or_reset = //
				sqlite3_column_count(stmt) ? sqlite3_reset : sqlite3_step;
		error.assign(step_or_reset(stmt), get_error_category());

		if (error.value() == SQLITE_DONE)
			error = boost::system::error_code();
	}

	void execute(const Param& params)
	{
		boost::system::error_code error;
		execute(params, error);

		if (error)
			throw std::runtime_error(error_message());
	}

	template<typename Handler>
	void async_execute(const Param& params, Handler handler)
	{
		post(boost::bind(&statement::execute, this, params, _1), handler);
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

		throw std::runtime_error(error_message());
	}

	// template<typename Handler>
	// void async_fetch(Result& result, Handler handler)
	// {
	//     post(boost::bind(statement::prepare, this, _1), handler);
	// }

private:

	template<typename Function, typename Callback>
	void post(Function function, Callback callback)
	{
		conn.get_io_service().post(sql::detail::handler<Function, Callback>(
				conn.get_io_service(), function, callback));
	}

	sqlite3_stmt* stmt;
	connection& conn;
	std::string query_str;
	bool prepared;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_STATEMENT_HPP*/
