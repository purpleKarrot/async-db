/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_DETAIL_SERVICE_HPP
#define BOOST_SQL_POSTGRES_DETAIL_SERVICE_HPP

#include <boost/sql/detail/service_base.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{
namespace detail
{

class service: public sql::detail::service_base<service>
{
public:
	service(asio::io_service& io_service) :
		sql::detail::service_base<service>(io_service)
	{
	}

	virtual ~service()
	{
	}

private:
	virtual void shutdown_service()
	{
	}
};

} // end namespace detail
} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_POSTGRES_DETAIL_SERVICE_HPP */
