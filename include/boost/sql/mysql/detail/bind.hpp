/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_BIND_HPP
#define BOOST_SQL_DETAIL_MYSQL_BIND_HPP

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

struct bind_param
{
	template<typename T>
	static void call(MYSQL_BIND& b, const T& value)
	{
		b.buffer = const_cast<void*> (reinterpret_cast<const void*> (&value));
	}

	static void call(MYSQL_BIND& b, const std::string& value)
	{
		b.buffer = const_cast<void*> (reinterpret_cast<const void*> (value.c_str()));
		*b.length = value.length();
	}

	template<typename T>
	static void call(MYSQL_BIND& b, const optional<T>& value)
	{
		*b.is_null = !value;
		if (value)
			call(b, *value);
	}

	static void call(MYSQL_BIND& b, const gregorian::date& value)
	{
		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);
		time->year = value.year();
		time->month = value.month();
		time->day = value.day();
		time->hour = 0;
		time->minute = 0;
		time->second = 0;
		time->second_part = 0;
		time->neg = 0;
		time->time_type = MYSQL_TIMESTAMP_DATE;
	}

	static void call(MYSQL_BIND& b, const posix_time::time_duration& value)
	{
		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);
		time->year = 0;
		time->month = 0;
		time->day = 0;
		time->hour = value.hours();
		time->minute = value.minutes();
		time->second = value.seconds();
		time->second_part = 0;
		time->neg = value.is_negative();
		time->time_type = MYSQL_TIMESTAMP_TIME;
	}

	static void call(MYSQL_BIND& b, const posix_time::ptime& value)
	{
		//call(b, value.date());
		//call(b, value.time_of_day());

		MYSQL_TIME* time = reinterpret_cast<MYSQL_TIME*> (b.buffer);
		time->year = value.date().year();
		time->month = value.date().month();
		time->day = value.date().day();
		time->hour = value.time_of_day().hours();
		time->minute = value.time_of_day().minutes();
		time->second = value.time_of_day().seconds();
		time->second_part = 0;
		time->neg = 0;
		time->time_type = MYSQL_TIMESTAMP_DATETIME;
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_BIND_HPP */
