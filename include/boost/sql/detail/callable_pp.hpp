/**************************************************************
 * Copyright (c) 2008-2010 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_PP_IS_ITERATING
#  error Do not include this file directly, use callable.hpp!
#endif

#define N BOOST_PP_ITERATION()

// Comma if nonzero number of arguments
#if N == 0
#  define BOOST_SQL_COMMA
#else
#  define BOOST_SQL_COMMA ,
#endif // N == 0

// if only one argument and argument is sequence, use as param_type
#if N == 1
#  define BOOST_SQL_PARAM_TYPE typename mpl::if_< \
     typename fusion::traits::is_sequence<Arg0>::type, \
     Arg0, fusion::vector<Arg0> >::type
#else
#  define BOOST_SQL_PARAM_TYPE fusion::vector<BOOST_PP_ENUM_PARAMS(N, Arg)>
#endif

#define BOOST_SQL_RESULT_TYPE typename mpl::if_< \
     typename fusion::traits::is_sequence<R>::type, \
     R, fusion::vector<R> >::type

template<typename Connection, template<typename , typename > class Statement,
		typename R BOOST_SQL_COMMA BOOST_PP_ENUM_PARAMS(N, typename Arg)>
class callable<Connection, Statement, R(BOOST_PP_ENUM_PARAMS(N, Arg)),
	typename enable_if<is_void<R> >::type>
{
public:
	callable(Connection& conn, const std::string& query) :
		impl(new impl_type(conn, query))
	{
	}

	callable(callable const& other) :
		impl(other.impl)
	{
	}

	void operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, Arg, arg))
	{
		impl->execute(BOOST_SQL_PARAM_TYPE(BOOST_PP_ENUM_PARAMS(N, arg)));
	}

private:
	typedef Statement<BOOST_SQL_PARAM_TYPE, fusion::vector<> > impl_type;
	boost::shared_ptr<impl_type> impl;
};

template<typename Connection, template<typename , typename > class Statement,
		typename R BOOST_SQL_COMMA BOOST_PP_ENUM_PARAMS(N, typename Arg)>
class callable<Connection, Statement, R(BOOST_PP_ENUM_PARAMS(N, Arg)),
	typename disable_if<is_void<R> >::type>
{
public:
	callable(Connection& conn, const std::string& query) :
		impl(new impl_type(conn, query))
	{
	}

	callable(callable const& other) :
		impl(other.impl)
	{
	}

	R operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, Arg, arg))
	{
		BOOST_SQL_RESULT_TYPE result;
		impl->execute(BOOST_SQL_PARAM_TYPE(BOOST_PP_ENUM_PARAMS(N, arg)));
		impl->fetch(result);
		return make_result<R, BOOST_SQL_RESULT_TYPE>::call(result);
	}

private:
	typedef Statement<BOOST_SQL_PARAM_TYPE, BOOST_SQL_RESULT_TYPE> impl_type;
	boost::shared_ptr<impl_type> impl;
};

#undef BOOST_SQL_RESULT_TYPE
#undef BOOST_SQL_PARAM_TYPE
#undef BOOST_SQL_COMMA
#undef N
