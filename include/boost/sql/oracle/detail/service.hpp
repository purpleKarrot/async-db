/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ORACLE_DETAIL_SERVICE_HPP
#define BOOST_SQL_ORACLE_DETAIL_SERVICE_HPP

#include <oci.h>
#include <boost/sql/detail/service_base.hpp>

namespace boost
{
namespace sql
{
namespace oracle
{
namespace detail
{

class service: public sql::detail::service_base<service>
{
public:
	service(asio::io_service& io_service) :
		sql::detail::service_base<service>(io_service)
	{
		if (OCIEnvCreate(&environment, OCI_DEFAULT, 0, 0, 0, 0, 0, 0))
			throw std::bad_alloc();
	}

	virtual ~service()
	{
	}

	OCIEnv* env()
	{
		return environment;
	}

private:
	virtual void shutdown_service()
	{
		OCIHandleFree(environment, OCI_HTYPE_ENV);
	}

	OCIEnv* environment;
};

} // end namespace detail
} // end namespace oracle
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ORACLE_DETAIL_SERVICE_HPP */
