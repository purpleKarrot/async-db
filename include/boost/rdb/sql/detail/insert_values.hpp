//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#define n BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(n, class Expr)>
typename with_values<
  typename fusion::result_of::make_vector<BOOST_PP_ENUM_PARAMS(n, Expr)>::type
>::type
values(BOOST_PP_ENUM_BINARY_PARAMS(n, const Expr, &expr)) {
  return with_values<
    typename fusion::result_of::make_vector<BOOST_PP_ENUM_PARAMS(n, Expr)>::type
  >::make(data_, fusion::make_vector(BOOST_PP_ENUM_PARAMS(n, expr)));
}
