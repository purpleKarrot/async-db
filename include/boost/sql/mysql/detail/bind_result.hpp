/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_DETAIL_BIND_RESULT_HPP
#define BOOST_SQL_MYSQL_DETAIL_BIND_RESULT_HPP

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

struct bind_result
{
	template<typename T>
	static void call(MYSQL_BIND& b, T& value)
	{
		b.buffer = reinterpret_cast<void*> (&value);
	}

	static void call(MYSQL_BIND& b, std::string& value)
	{
		b.buffer = (void*) value.c_str();
		b.buffer_length = value.length();
	}

	template<typename T>
	static void call(MYSQL_BIND& b, optional<T>& value)
	{
		value = T();
		call(b, *value);
	}

	static void call(MYSQL_BIND& b, gregorian::date& value)
	{
	}

	static void call(MYSQL_BIND& b, posix_time::time_duration& value)
	{
	}

	static void call(MYSQL_BIND& b, posix_time::ptime& value)
	{
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_DETAIL_BIND_RESULT_HPP */
