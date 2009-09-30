/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_FINALIZE_HPP
#define BOOST_SQL_DETAIL_MYSQL_FINALIZE_HPP

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/optional.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

struct finalize
{
	template<typename T>
	static void call(MYSQL_BIND& bind, const T)
	{
	}

	template<typename T>
	static void call(MYSQL_BIND& bind, const optional<T>& value)
	{
		delete bind.is_null;
		call(bind, *value);
	}

	static void call(MYSQL_BIND& bind, const std::string&)
	{
		delete bind.length;
	}

	static void call(MYSQL_BIND& bind, const gregorian::date&)
	{
		delete reinterpret_cast<MYSQL_TIME*> (bind.buffer);
	}

	static void call(MYSQL_BIND& bind, const posix_time::time_duration&)
	{
		delete reinterpret_cast<MYSQL_TIME*> (bind.buffer);
	}

	static void call(MYSQL_BIND& bind, const posix_time::ptime&)
	{
		delete reinterpret_cast<MYSQL_TIME*> (bind.buffer);
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_FINALIZE_HPP */
