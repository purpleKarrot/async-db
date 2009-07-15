/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_DETAIL_MYSQL_RESIZE_HPP
#define BOOST_SQL_DETAIL_MYSQL_RESIZE_HPP

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

struct resize_param
{
	template<typename T>
	static void call(MYSQL_BIND& bind, T)
	{
	}

	template<typename T>
	static void call(MYSQL_BIND& bind, optional<T>& dummy)
	{
		call(bind, *dummy);
	}

	static void call(MYSQL_BIND& bind, std::string& value)
	{
		value.resize(*bind.length);
		bind.buffer = (void*) value.c_str();
		bind.buffer_length = value.length();
	}
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_RESIZE_HPP */
