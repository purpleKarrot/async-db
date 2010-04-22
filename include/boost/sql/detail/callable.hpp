/**************************************************************
 * Copyright (c) 2008-2010 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_CALLABLE_HPP
#define BOOST_SQL_DETAIL_CALLABLE_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <string>
#include <boost/shared_ptr.hpp>

namespace boost
{
namespace sql
{
namespace detail
{

template<typename R, typename Seq>
struct make_result
{
	static R call(Seq& seq)
	{
		return fusion::front(seq);
	}
};

template<typename R>
struct make_result<R, R>
{
	static R call(R& seq)
	{
		return seq;
	}
};

template<typename Connection, template<typename , typename > class Statement,
		typename Signature, typename Enable = void>
struct callable;

#define BOOST_PP_FILENAME_1 <boost/sql/detail/callable_pp.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_VECTOR_SIZE)
#include BOOST_PP_ITERATE()

} // end namespace detail
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_CALLABLE_HPP */
