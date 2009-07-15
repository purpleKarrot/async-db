/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_FIREBIRD_CONNECTION_HPP
#define BOOST_SQL_FIREBIRD_CONNECTION_HPP

#include <boost/sql/firebird/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>
#include <string>
#include <stdexcept>
#include <boost/assert.hpp>
#include <sstream>
#include <ibase.h>

namespace boost
{
namespace sql
{
namespace firebird
{

class connection: sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service), impl(0)
	{
	}

	~connection()
	{
		isc_detach_database(service.status, &impl);
	}

	void open(std::string const& db_name, std::string const& parm_buffer)
	{
		if (!isc_attach_database(service.status, db_name.length(),
				db_name.c_str(), &impl, parm_buffer.length(),
				parm_buffer.c_str()))
		{
			service.throw_error();
		}
	}

	unsigned long client_version()
	{
		return 100 * isc_get_client_major_version()
				+ isc_get_client_minor_version();
	}

	unsigned long server_version()
	{
		return 0;
	}

	void execute(const std::string& query)
	{
	}

private:
	isc_db_handle impl;
};

} // end namespace firebird
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_CONNECTION_HPP*/
