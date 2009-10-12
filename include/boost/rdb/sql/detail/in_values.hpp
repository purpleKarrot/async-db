//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#define n BOOST_PP_ITERATION()

    template<BOOST_PP_ENUM_PARAMS(n, class Expr)>
    expression<
      sql::in_values<Expr,
        typename fusion::result_of::make_vector<
          BOOST_PP_REPEAT(n, BOOST_RDB_RESULT_OF_MAKE_EXPRESSION, Expr)
        >::type
      >
    >
    in(BOOST_PP_ENUM_BINARY_PARAMS(n, const Expr, &expr)) {
      return expression<
        sql::in_values<Expr,
          typename fusion::result_of::make_vector<
            BOOST_PP_REPEAT(n, BOOST_RDB_RESULT_OF_MAKE_EXPRESSION, Expr)
          >::type
        >
      >(*this, fusion::make_vector(
        BOOST_PP_REPEAT(n, BOOST_RDB_MAKE_EXPRESSION, expr)
        ));
    }
