/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_DETAIL_ERROR_HPP
#define BOOST_SQL_MYSQL_DETAIL_ERROR_HPP

#include <errmsg.h>
#include <boost/system/system_error.hpp>
#include <sstream>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

struct error_category: boost::system::error_category
{
	const char* name() const
	{
		return "MySQL";
	}

	std::string message(int value) const
	{
		std::stringstream str;
		str << name() << ':' << value;
		return str.str();
	}
};

} // namespace detail

boost::system::error_category& get_error_category()
{
	static detail::error_category category;
	return category;
}

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_DETAIL_ERROR_HPP */
