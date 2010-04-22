/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_CONNECTION_HPP
#define BOOST_SQL_POSTGRES_CONNECTION_HPP

#include <libpq-fe.h>
#include <boost/sql/postgres/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/sql/detail/callable.hpp>
#include <boost/function.hpp>
#include <stdexcept>
#include <pg_config.h>

namespace boost
{
namespace sql
{
namespace postgres
{

template<typename Param, typename Result>
class statement;

class connection: sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service), impl(0),
				socket(io_service)
	{
	}

	~connection()
	{
		PQfinish(impl);
	}

	void open(const char* conninfo)
	{
		impl = PQconnectdb(conninfo);
		if (PQstatus(impl) != CONNECTION_OK)
			throw std::runtime_error(PQerrorMessage(impl));

		socket.assign(asio::ip::tcp::v4(), PQsocket(impl));
	}

	unsigned long client_version()
	{
		return PG_VERSION_NUM;
	}

	unsigned long server_version()
	{
		return PQserverVersion(impl);
	}

	void execute(const std::string& cmd)
	{
		boost::shared_ptr<PGresult> res(PQexec(impl, cmd.c_str()), PQclear);
		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
		{
			throw std::runtime_error(PQresultErrorMessage(res.get()));
		}
	}

	template<typename Signature>
	boost::function<Signature> prepare(std::string const& query)
	{
		return sql::detail::callable<connection, statement, Signature>(*this,
				query);
	}

	PGconn* implementation() const
	{
		return impl;
	}

private:

	void read_handler(const system::error_code& ec)
	{
		if (ec)
		{
			//ois.post(Callback, ec);
		}

		if (!PQconsumeInput(impl))
		{
			//ois.post(Callback, error);
		}

		if (!PQisBusy(impl))
		{
			PGresult * res = PQgetResult(impl);
			//ois.post(Callback, result(res));
		}

		socket.async_read_some(boost::asio::null_buffers(), //
				boost::bind(&connection::read_handler, this, _1));
	}

	PGconn* impl;
	asio::ip::tcp::socket socket;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_CONNECTION_HPP*/
