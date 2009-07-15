/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_MYSQL_PARAMETERS_HPP
#define BOOST_SQL_MYSQL_PARAMETERS_HPP

#include <mysql.h>

#include <boost/sql/mysql/detail/algorithm.hpp>
#include <boost/sql/mysql/detail/prepare.hpp>
#include <boost/sql/mysql/detail/finalize.hpp>
#include <boost/sql/mysql/detail/bind.hpp>

#include <boost/array.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

template<typename Seq, typename Enable = void>
struct parameters
{
	void bind_param(MYSQL_STMT*, const Seq&)
	{
	}
};

template<typename Seq>
struct parameters<Seq, typename enable_if<typename mpl::not_<
		typename mpl::empty<Seq>::type>::type>::type>
{
	parameters()
	{
		memset(bind_input, 0, sizeof(bind_input));
		mpl::for_each<Seq>(algorithm<prepare> (bind_input));
	}

	~parameters()
	{
		mpl::for_each<Seq>(algorithm<finalize> (bind_input));
	}

	void bind_param(MYSQL_STMT* stmt, const Seq& params)
	{
		fusion::for_each(params, algorithm<detail::bind_param> (bind_input));

		if (mysql_stmt_bind_param(stmt, bind_input))
			throw std::runtime_error(mysql_stmt_error(stmt));
	}

	enum
	{
		size = mpl::size<Seq>::value
	};

	/* The MYSQL_BIND array is only pointed to by the MYSQL_STMT so it must not
	 * be changed or freed until the MYSQL_STMT is not used any more */
	MYSQL_BIND bind_input[size];
};

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_PARAMETERS_HPP*/
