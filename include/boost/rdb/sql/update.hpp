//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_UPDATE_HPP
#define BOOST_RDB_UPDATE_HPP

#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/sql/expression.hpp>

namespace boost { namespace rdb { namespace sql {

  template<class Dialect, class State, class Data, class Subdialect>
  struct update_statement :
    tag_if<fusion::result_of::has_key<Data, typename Subdialect::set>, update_statement_tag> {

    explicit update_statement(const Data& data) : data_(data) { }

    typedef void result;
    typedef typename placeholders_from_pair_list<Data>::type placeholders;

    Data data_;

    template<class K, class T, class D = Data>
    struct transition {
      typedef update_statement<
        Subdialect,
        K,
        typename result_of::add_key<
          D,
          K,
          T
        >::type,
        Subdialect
      > type;
    };

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/update_set.hpp>
    #include BOOST_PP_ITERATE()
    
    #include "detail/select_where.hpp"

    void str(std::ostream& os) const {
      fusion::for_each(data_, str_clause(os));
    }
  };

  BOOST_RDB_ALLOW(sql2003, set, where);

  template<class Table>

  inline void str(std::ostream& os, const fusion::pair<sql2003::update, const Table*>& p) {
    os << "update ";
    os << p.second->table();
  }

  template<class AssignList>
  inline void str(std::ostream& os, const fusion::pair<sql2003::set, AssignList>& p) {
    os << " set ";
    fusion::for_each(p.second, comma_output(os));
  }

  struct extract_placeholders_from_assign {

    template<typename Sig>
    struct result;

    template<class Self, class Col, class Expr, class Placeholders>
    struct result<Self(set_clause<Col, Expr>&, Placeholders&)> {
      typedef typename mpl::if_<
        is_placeholder<Expr>,
        typename fusion::result_of::push_back<
          Placeholders,
          typename Col::sql_type
        >::type,
        Placeholders
      >::type type;
    };
  };

  template<class AssignList>
  struct extract_placeholders_from_pair<sql2003::set, AssignList> {
    typedef typename fusion::result_of::as_vector<
      typename fusion::result_of::accumulate<AssignList, fusion::vector<>, extract_placeholders_from_assign>::type
    >::type type;
  };

  template<class Table>
  update_statement<
    sql2003,
    sql2003::update,
    fusion::map<
      fusion::pair<
      sql2003::update, const Table*
      >
    >,
    sql2003
  >
  update(const Table& table) {
    return update_statement<
      sql2003,
      sql2003::update,
      fusion::map<
        fusion::pair<
        sql2003::update, const Table*
        >
      >,
      sql2003
    >(fusion::make_pair<sql2003::update>(&table));
  }

} } }

#endif
