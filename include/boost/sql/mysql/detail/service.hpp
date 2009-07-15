/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_DETAIL_SERVICE_HPP
#define BOOST_SQL_MYSQL_DETAIL_SERVICE_HPP

//#include <my_global.h>
#include <mysql.h>
#include <stdexcept>
#include <boost/sql/detail/service_base.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

class service: public sql::detail::service_base<service>
{
public:
	service(asio::io_service& io_service) :
		sql::detail::service_base<service>(io_service)
	{
		if (mysql_library_init(0, 0, 0))
			throw std::bad_alloc();
	}

	virtual ~service()
	{
	}

private:
	virtual void shutdown_service()
	{
		mysql_library_end();
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_DETAIL_SERVICE_HPP */
