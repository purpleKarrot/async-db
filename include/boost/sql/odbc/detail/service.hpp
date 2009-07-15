/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_DETAIL_SERVICE_HPP
#define BOOST_SQL_ODBC_DETAIL_SERVICE_HPP

#include <sql.h>
#include <sqlext.h>
#include <boost/sql/odbc/detail/throw_error.hpp>
#include <boost/sql/detail/service_base.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{
namespace detail
{

class service: public sql::detail::service_base<service>
{
public:
	service(asio::io_service& io_service) :
		sql::detail::service_base<service>(io_service)
	{
		if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,
				&environment)))
			throw std::bad_alloc();

		if (!SQL_SUCCEEDED(SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION,
						(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER)))
			detail::throw_error(SQL_HANDLE_ENV, environment);
	}

	~service()
	{
	}

	HENV environment;

private:
	void shutdown_service()
	{
		SQLFreeHandle(SQL_HANDLE_ENV, environment);
	}
};

} // end namespace detail
} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_DETAIL_SERVICE_HPP */
