/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_FETCH_HPP
#define BOOST_SQL_DETAIL_MYSQL_FETCH_HPP

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

struct fetch_param
{
	template<typename T>
	static void call(MYSQL_BIND& b, T& value)
	{
	}

	static void call(MYSQL_BIND& b, std::string& value)
	{
		value.resize(*b.length);
	}

	template<typename T>
	static void call(MYSQL_BIND& b, boost::optional<T>& value)
	{
		if (*b.is_null)
			value = boost::optional<T>();
		else
		{
			T v;
			call(b, v);
			value = v;
		}
	}

	static void call(MYSQL_BIND& b, boost::gregorian::date& value)
	{
		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);

		value = boost::gregorian::date(time->year, time->month, time->day);
	}

	static void call(MYSQL_BIND& b, boost::posix_time::time_duration& value)
	{
		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);

		value = boost::posix_time::time_duration(time->hour, time->minute,
				time->second, time->second_part);
	}

	static void call(MYSQL_BIND& b, boost::posix_time::ptime& value)
	{
		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);

		value = boost::posix_time::ptime(boost::gregorian::date(time->year,
				time->month, time->day), boost::posix_time::time_duration(
				time->hour, time->minute, time->second, time->second_part));
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_FETCH_HPP */
