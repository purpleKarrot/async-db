//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#define n BOOST_PP_ITERATION()

    template<BOOST_PP_ENUM_PARAMS(n, class Set)>
    typename transition<
      typename Subdialect::set,
      fusion::vector<
        BOOST_PP_ENUM_PARAMS(n, Set)
      >
    >::type
    set(BOOST_PP_ENUM_BINARY_PARAMS(n, const Set, &set)) {
      return typename transition<
        typename Subdialect::set,
        fusion::vector<
          BOOST_PP_ENUM_PARAMS(n, Set)
        >
      >::type(add_key<Subdialect::set>(data_, fusion::make_vector(BOOST_PP_ENUM_PARAMS(n, set))));
    }
