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
#include <boost/sql/mysql/detail/error.hpp>
#include <boost/sql/detail/handler.hpp>
#include <boost/mpl/size.hpp>
#include <boost/bind.hpp>

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
		io_service(conn.get_io_service()), query_str(query), prepared(false)
	{
		impl = mysql_stmt_init(conn.implementation());
		if (!impl)
			throw std::bad_alloc();
	}

	~statement()
	{
		mysql_stmt_close(impl);
	}

	std::string error_message() const
	{
		return mysql_stmt_error(impl);
	}

	void prepare(boost::system::error_code& error)
	{
		if (mysql_stmt_prepare(impl, query_str.c_str(), query_str.length()))
		{
			error.assign(mysql_stmt_errno(impl), get_error_category());
		}
		else
		{
			BOOST_ASSERT(mysql_stmt_param_count(impl) == param_count);
			BOOST_ASSERT(mysql_stmt_field_count(impl) == result_count);

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
			{
				std::cout << error << std::endl;
				return;
			}
		}

		bind_param(impl, params);

		if (mysql_stmt_execute(impl) || mysql_stmt_store_result(impl))
			error.assign(mysql_stmt_errno(impl), get_error_category());
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

	bool fetch(Result& results)
	{
		bind_result(impl, results);

		MYSQL_ROW_OFFSET offset = mysql_stmt_row_tell(impl);
		int error = mysql_stmt_fetch(impl);
		if (error == MYSQL_DATA_TRUNCATED)
		{
			resize_result(impl, results);
			bind_result(impl, results);
			mysql_stmt_row_seek(impl, offset);
			error = mysql_stmt_fetch(impl);
		}

		if (!error)
		{
			// TODO: trim strings to the right length!
			return true;
		}

		if (error == MYSQL_NO_DATA)
			return false;

		throw_error("mysql_stmt_fetch");
	}

private:

	template<typename Function, typename Callback>
	void post(Function function, Callback callback)
	{
		io_service.post(sql::detail::handler<Function, Callback>(io_service,
				function, callback));
	}

	void throw_error(const char* const function)
	{
		std::stringstream err;
		err << "Error " << mysql_stmt_errno(impl) << " in " << function << ": "
				<< mysql_stmt_error(impl);
		throw std::runtime_error(err.str());
	}

	asio::io_service& io_service;
	MYSQL_STMT* impl;
	std::string query_str;
	bool prepared;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_STATEMENT_HPP*/
