/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_MYSQL_DETAIL_BINDING_HPP
#define BOOST_SQL_MYSQL_DETAIL_BINDING_HPP

#include <mysql.h>

#include <boost/sql/mysql/detail/algorithm.hpp>
#include <boost/sql/mysql/detail/prepare.hpp>
#include <boost/sql/mysql/detail/finalize.hpp>
#include <boost/sql/mysql/detail/bind_param.hpp>
#include <boost/sql/mysql/detail/bind_result.hpp>
#include <boost/sql/mysql/detail/fetch.hpp>
#include <boost/sql/mysql/detail/resize.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

template<typename Sequence, typename Enable = void>
struct binding
{
};

template<typename Sequence>
struct binding<Sequence, typename disable_if<
		typename mpl::empty<Sequence>::type>::type>
{
	binding()
	{
		memset(impl, 0, sizeof(impl));
		mpl::for_each<Sequence>(algorithm<prepare> (impl));
	}

	~binding()
	{
		mpl::for_each<Sequence>(algorithm<finalize> (impl));
	}

	void bind_param(const Sequence& sequence)
	{
		fusion::for_each(sequence, algorithm<detail::bind_param> (impl));
	}

	void bind_result(Sequence& sequence)
	{
		fusion::for_each(sequence, algorithm<detail::bind_result> (impl));
	}

	void fetch(Sequence& sequence)
	{
		fusion::for_each(sequence, algorithm<detail::fetch_param> (impl));
	}

	void resize(Sequence& sequence)
	{
		fusion::for_each(sequence, algorithm<detail::resize_param> (impl));
	}

	/* The MYSQL_BIND array is only pointed to by the MYSQL_STMT so it must not
	 * be changed or freed until the MYSQL_STMT is not used any more */
	MYSQL_BIND impl[mpl::size<Sequence>::value];
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_MYSQL_DETAIL_BINDING_HPP */
