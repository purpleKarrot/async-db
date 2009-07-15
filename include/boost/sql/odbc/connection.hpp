/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_CONNECTION_HPP
#define BOOST_SQL_ODBC_CONNECTION_HPP

#include <boost/sql/odbc/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{

class connection: sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service)
	{
		if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, service.environment,
						&impl)))
		{
			detail::throw_error(SQL_HANDLE_ENV, service.environment);
		}
	}

	~connection()
	{
		SQLFreeHandle(SQL_HANDLE_DBC, impl);
	}

	void open(const std::string& conn_str)
	{
		SQLTCHAR conn_out[SQL_MAX_OPTION_STRING_LENGTH];
		SQLSMALLINT cb;

		if (!SQL_SUCCEEDED(SQLDriverConnect(impl, NULL,
						(SQLCHAR*) conn_str.c_str(),
						conn_str.length(), conn_out, sizeof(conn_out), &cb,
						SQL_DRIVER_NOPROMPT)))
		{
			detail::throw_error(SQL_HANDLE_DBC, impl);
		}
	}

	unsigned long client_version()
	{
		return 0;
	}

	unsigned long server_version()
	{
		return 0;
	}

	void execute(const std::string& query)
	{
	}

	HDBC implementation()
	{
		return impl;
	}

private:
	HDBC impl;
};

} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_CONNECTION_HPP */
