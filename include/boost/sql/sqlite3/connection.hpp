/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_SQLITE3_CONNECTION_HPP
#define BOOST_SQL_SQLITE3_CONNECTION_HPP

#include <boost/sql/sqlite3/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>
#include <boost/sql/detail/callable.hpp>
#include <boost/function.hpp>
#include <stdexcept>
#include <exception>

namespace boost
{
namespace sql
{
namespace sqlite3
{

template<typename Param, typename Result>
class statement;

class connection: public sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service), impl(0)
	{
	}

	~connection()
	{
		sqlite3_close(impl);
	}

	void open(const char* name)
	{
		if (sqlite3_open(name, &impl) != SQLITE_OK)
			throw std::runtime_error(sqlite3_errmsg(impl));

		if (!impl)
			throw std::bad_alloc();
	}

	unsigned long client_version()
	{
		return SQLITE_VERSION_NUMBER;
	}

	unsigned long server_version()
	{
		return sqlite3_libversion_number();
	}

	template<typename Signature>
	boost::function<Signature> prepare(std::string const& query)
	{
		return sql::detail::callable<connection, statement, Signature>(*this,
				query);
	}

	void execute(const std::string& cmd)
	{
		prepare<void()>(cmd)();
	}

	::sqlite3* implementation() const
	{
		return impl;
	}

private:
	::sqlite3* impl;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_CONNECTION_HPP*/
