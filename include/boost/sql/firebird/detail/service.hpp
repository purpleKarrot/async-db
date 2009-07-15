/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_FIREBIRD_DETAIL_SERVICE_HPP
#define BOOST_SQL_FIREBIRD_DETAIL_SERVICE_HPP

#include <ibase.h>
#include <boost/sql/detail/service_base.hpp>

namespace boost
{
namespace sql
{
namespace firebird
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

	void throw_error()
	{
		BOOST_ASSERT(status[0] == 1);
		BOOST_ASSERT(status[1] != 0);

		char buf[512];
		std::stringstream err;
		const ISC_STATUS * pvector = status;

		while (fb_interpret(buf, sizeof(buf), &pvector))
			err << buf << "\n";

		throw std::runtime_error(err.str());
	}

	ISC_STATUS_ARRAY status;

private:
	virtual void shutdown_service()
	{
	}
};

} // end namespace detail
} // end namespace firebird
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_FIREBIRD_DETAIL_SERVICE_HPP */
