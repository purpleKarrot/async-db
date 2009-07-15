/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_STATEMENT_HPP
#define BOOST_SQL_MYSQL_STATEMENT_HPP

#include <boost/sql/mysql/connection.hpp>
#include <boost/sql/mysql/detail/parameters.hpp>
#include <boost/sql/mysql/detail/results.hpp>
#include <boost/mpl/size.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

template<typename Param, typename Result>
class statement: public detail::parameters<Param>, public detail::results<
		Result>
{
	enum
	{
		param_count = mpl::size<Param>::value,
		result_count = mpl::size<Result>::value
	};

public:
	statement(connection& conn, const std::string& query) :
		query_str(query), prepared(false)
	{
		impl = mysql_stmt_init(conn.implementation());
		if (!impl)
			throw std::bad_alloc();
	}

	~statement()
	{
		mysql_stmt_close(impl);
	}

	void prepare()
	{
		if (mysql_stmt_prepare(impl, query_str.c_str(), query_str.length()))
			throw_error("mysql_stmt_prepare");

		BOOST_ASSERT(mysql_stmt_param_count(impl) == param_count);
		BOOST_ASSERT(mysql_stmt_field_count(impl) == result_count);

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		bind_param(impl, params);

		if (mysql_stmt_execute(impl))
			throw_error("mysql_stmt_execute");
	}

	bool fetch(Result& results)
	{
		bind_result(impl, results);

		int error = mysql_stmt_fetch(impl);
		if (error == MYSQL_DATA_TRUNCATED)
		{
			resize_result(impl, results);
			error = mysql_stmt_fetch(impl);
		}

		if (!error)
			return true;

		if (error == MYSQL_NO_DATA)
			return false;

		throw_error("mysql_stmt_fetch");
	}

private:
	void throw_error(const char* const function)
	{
		std::stringstream err;
		err << "Error " << mysql_stmt_errno(impl) << " in " << function << ": "
				<< mysql_stmt_error(impl);
		throw std::runtime_error(err.str());
	}

	MYSQL_STMT* impl;
	std::string query_str;
	bool prepared;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_STATEMENT_HPP*/
