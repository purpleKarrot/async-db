/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_ALGORITHM_HPP
#define BOOST_SQL_DETAIL_MYSQL_ALGORITHM_HPP

#include <mysql.h>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

template<typename C>
class algorithm
{
public:
	algorithm(MYSQL_BIND* bind) :
		bind(bind)
	{
	}

	template<typename T>
	void operator()(T&value) const
	{
		C::call(*bind, value);
		++bind;
	}

private:
	mutable MYSQL_BIND* bind;
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_MYSQL_ALGORITHM_HPP */
