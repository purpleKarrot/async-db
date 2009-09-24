/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_DETAIL_SERVICE_RUNNER_HPP
#define BOOST_SQL_MYSQL_DETAIL_SERVICE_RUNNER_HPP

#include <mysql.h>
#include <boost/asio/io_service.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

class service_runner
{
public:
	service_runner(boost::asio::io_service& io_service) :
		io_service_(io_service)
	{
	}

	void operator()()
	{
		mysql_thread_init();
		io_service_.run();
		mysql_thread_end();
	}

private:
	boost::asio::io_service & io_service_;
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_DETAIL_SERVICE_RUNNER_HPP */
