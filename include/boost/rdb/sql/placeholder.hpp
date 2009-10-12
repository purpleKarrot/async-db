//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_PLACEHOLDER_HPP
#define BOOST_RDB_PLACEHOLDER_HPP

#include <boost/rdb/sql/common.hpp>

namespace boost { namespace rdb { namespace sql {

  template<int N>
  struct placeholder {
    typedef placeholder_type sql_type;
    enum { precedence = precedence_level::highest };
    void str(std::ostream& os) const {
      os << "%";
    }
  };

  const expression< placeholder<0> > _;

  //struct make_placeholders_for_expr {

  //  template<typename Sig>
  //  struct result;

  //  template<typename Self, typename Expr>
  //  struct result<Self(Expr)>
  //  {
  //    typedef typename boost::remove_reference<Expr>::type::cpp_type type;
  //  };
  //};

  //namespace result_of {
  //  template<class ExprList>
  //  struct make_placeholders_for_list {
  //    typedef typename fusion::result_of::as_vector<
  //      typename fusion::result_of::transform<ExprList, make_placeholders_for_expr>::type
  //    >::type type;
  //  };
  //}

} } }

#endif
