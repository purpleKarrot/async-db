/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_MYSQL_FIELD_TYPE_HPP
#define BOOST_SQL_DETAIL_MYSQL_FIELD_TYPE_HPP

#include <mysql.h>
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/assert.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

template<enum_field_types Value>
struct field_type_base
{
	BOOST_STATIC_CONSTANT(enum_field_types, value = Value);
};

typedef mpl::map< //
		mpl::pair<mpl::int_<1>, field_type_base<MYSQL_TYPE_TINY> >, //
		mpl::pair<mpl::int_<2>, field_type_base<MYSQL_TYPE_SHORT> >, //
		mpl::pair<mpl::int_<4>, field_type_base<MYSQL_TYPE_LONG> >, //
		mpl::pair<mpl::int_<8>, field_type_base<MYSQL_TYPE_LONGLONG> > //
>::type field_type_map;

template<typename T>
struct field_type: mpl::at<field_type_map, mpl::int_<sizeof(T)> >::type
{
	BOOST_MPL_ASSERT(( is_integral<T> ));
};

template<>
struct field_type<float> : field_type_base<MYSQL_TYPE_FLOAT>
{
};

template<>
struct field_type<double> : field_type_base<MYSQL_TYPE_DOUBLE>
{
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_DETAIL_MYSQL_FIELD_TYPE_HPP*/
