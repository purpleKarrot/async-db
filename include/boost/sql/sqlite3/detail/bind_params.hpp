/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_SQLITE3_BIND_PARAMS_HPP
#define BOOST_SQL_DETAIL_SQLITE3_BIND_PARAMS_HPP

#include <sqlite3.h>

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/view/reverse_view.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/mpl/size.hpp>
#include <boost/optional.hpp>
#include <string>

namespace boost
{
namespace sql
{
namespace sqlite3
{
namespace detail
{

inline int bind(sqlite3_stmt* stmt, int pos, int value)
{
	return sqlite3_bind_int(stmt, pos, value);
}

inline int bind(sqlite3_stmt* stmt, int pos, sqlite_int64 value)
{
	return sqlite3_bind_int64(stmt, pos, value);
}

inline int bind(sqlite3_stmt* stmt, int pos, double value)
{
	return sqlite3_bind_double(stmt, pos, value);
}

inline int bind(sqlite3_stmt* stmt, int pos, const std::string& value)
{
	return sqlite3_bind_text(stmt, pos, value.c_str(), value.length(),
			SQLITE_TRANSIENT);
}

template<typename T>
inline int bind(sqlite3_stmt* stmt, int pos, const optional<T>& value)
{
	return (value) ? bind(stmt, pos, *value) : sqlite3_bind_null(stmt, pos);
}

template<int N>
struct bind_params_unrolled
{
	template<typename I0>
	static int call(sqlite3_stmt* stmt, I0 const& i0)
	{
		int rc = bind(stmt, N, *i0);
		if (rc)
			return rc;

		typedef	typename fusion::result_of::next<I0>::type I1;
		I1 i1(fusion::next( i0));
		rc = bind(stmt, N - 1, *i1);
		if (rc)
			return rc;

		typedef typename fusion::result_of::next<I1>::type I2;
		I2 i2(fusion::next( i1));
		rc = bind(stmt, N - 2, *i2);
		if (rc)
			return rc;

		return bind_params_unrolled<N - 3>::call(stmt, fusion::next(i2));
	}
};

template<>
struct bind_params_unrolled<2>
{
	template<typename I0>
	static int call(sqlite3_stmt* stmt, I0 const& i0)
	{
		int rc = bind(stmt, 2, *i0);
		if (rc)
			return rc;

		typedef typename fusion::result_of::next<I0>::type I1;
		I1 i1(fusion::next(i0));
		return bind(stmt, 1, *i1);
	}
};

template<>
struct bind_params_unrolled<1>
{
	template<typename I0>
	static int call(sqlite3_stmt* stmt, I0 const& i0)
	{
		return bind(stmt, 1, *i0);
	}
};

template<>
struct bind_params_unrolled<0>
{
	template<typename It>
	static int call(sqlite3_stmt*, It const&)
	{
		return 0;
	}
};

template<typename Sequence>
inline int bind_params(sqlite3_stmt* stmt, Sequence& seq)
{
	typedef typename mpl::size<Sequence>::type size;
	return bind_params_unrolled<size::value>::call(stmt,
			fusion::begin(fusion::reverse_view<Sequence>(seq)));
}

} // end namespace detail
} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_SQLITE3_BIND_PARAMS_HPP */
