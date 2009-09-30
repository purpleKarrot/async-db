/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_PREPARE_HPP
#define BOOST_SQL_DETAIL_MYSQL_PREPARE_HPP

#include <boost/sql/mysql/detail/field_type.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/type_traits.hpp>
#include <boost/optional.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

struct prepare
{
	template<typename T>
	static void call(MYSQL_BIND& bind, const T)
	{
		bind.buffer_type = field_type<T>::value;
		bind.buffer_length = sizeof(T);
		bind.is_unsigned = boost::is_unsigned<T>::value;
	}

	template<typename T>
	static void call(MYSQL_BIND& bind, const optional<T>& value)
	{
		bind.is_null = new my_bool;
		call(bind, T());
	}

	static void call(MYSQL_BIND& bind, const std::string&)
	{
		bind.buffer_type = MYSQL_TYPE_VAR_STRING;
		bind.length = new unsigned long;
	}

	static void call(MYSQL_BIND& bind, const boost::gregorian::date&)
	{
		bind.buffer_type = MYSQL_TYPE_DATE;
		bind.buffer = new MYSQL_TIME;
	}

	static void call(MYSQL_BIND& bind, const boost::posix_time::time_duration&)
	{
		bind.buffer_type = MYSQL_TYPE_TIME;
		bind.buffer = new MYSQL_TIME;
	}

	static void call(MYSQL_BIND& bind, const boost::posix_time::ptime&)
	{
		bind.buffer_type = MYSQL_TYPE_DATETIME;
		bind.buffer = new MYSQL_TIME;
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_PREPARE_HPP */
