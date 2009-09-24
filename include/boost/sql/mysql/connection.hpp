/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_CONNECTION_HPP
#define BOOST_SQL_MYSQL_CONNECTION_HPP

#include <boost/sql/mysql/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>
#include <string>

namespace boost
{
namespace sql
{
namespace mysql
{

class connection: public sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service)
	{
		impl = mysql_init(0);
		if (!impl)
			throw std::bad_alloc();
	}

	~connection()
	{
		mysql_close(impl);
	}

	void open(const char* db = 0, const char* user = 0, const char* passwd = 0,
			const char* host = 0, unsigned int port = 0,
			const char* unix_socket = 0, unsigned long clientflag = 0)
	{
		if (!mysql_real_connect(impl, host, user, passwd, db, port,
				unix_socket, clientflag))
		{
			throw std::runtime_error(mysql_error(impl));
		}
	}

	unsigned long client_version()
	{
		return mysql_get_client_version();
	}

	unsigned long server_version()
	{
		return mysql_get_server_version(impl);
	}

	void execute(const std::string& query)
	{
		if (mysql_real_query(impl, query.c_str(), query.length()))
			throw std::runtime_error(mysql_error(impl));
	}

	MYSQL* implementation()
	{
		return impl;
	}

private:
	MYSQL* impl;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_CONNECTION_HPP*/
