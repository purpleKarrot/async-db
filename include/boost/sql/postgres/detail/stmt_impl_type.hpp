/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_DETAIL_STMT_IMPL_TYPE_HPP
#define BOOST_SQL_POSTGRES_DETAIL_STMT_IMPL_TYPE_HPP

#include <string>
#include <boost/lexical_cast.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{
namespace detail
{

struct dummy
{
};

// Generate names for prepared statements
template<typename T>
class stmt_impl_type_base: public std::string
{
public:
	stmt_impl_type_base() :
		std::string("boost_sql_stmt_" + boost::lexical_cast<std::string>(
				counter++))
	{
	}

private:
	static int counter;
};

template<typename T>
int stmt_impl_type_base<T>::counter = 0;

typedef stmt_impl_type_base<dummy> stmt_impl_type;

} // end namespace detail
} // end namespace postgres
} // end namespace sql
} // end namespace boost


#endif /* BOOST_SQL_POSTGRES_DETAIL_STMT_IMPL_TYPE_HPP */
