//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_SELECT_HPP
#define BOOST_RDB_SELECT_HPP

#include <boost/rdb/sql/common.hpp>

namespace boost { namespace rdb { namespace sql {

  struct make_row {

    template<typename Sig>
    struct result;

    template<typename Self, typename Expr>
    struct result<Self(Expr)>
    {
      typedef typename boost::remove_reference<Expr>::type::cpp_type type;
    };
  };

  template<class SelectList>
  struct select_row {
    typedef typename fusion::result_of::as_vector<
      typename fusion::result_of::transform<SelectList, make_row>::type
    >::type type;
  };

  extern select_statement<sql2003, sql2003::select, fusion::map<>, sql2003> select;
  
  template<class Data, class Key, class Enable = void>
  struct select_result_if {
    select_result_if(const Data& data) : data_(data) { }
    Data data_;
  };
  
  template<class Data, class Key>
  struct select_result_if<
    Data,
    Key,
    typename enable_if<
      fusion::result_of::has_key<Data, Key>
    >::type
  > {

    typedef typename fusion::result_of::value_at_key<Data, Key>::type select_list;
    typedef nullable<typename select_row<select_list>::type> row;
    typedef std::deque<row> result;

    select_result_if(const Data& data) : data_(data) { }
    Data data_;

    const select_list& exprs() const {
      return fusion::at_key<Key>(data_);
    }
  };

  template<class ExprList>
  struct extract_placeholders_from_pair<sql2003::exprs, ExprList> {
    typedef typename placeholders_from_list<ExprList>::type type;
  };

  template<class Predicate>
  struct extract_placeholders_from_pair<sql2003::where, Predicate> {
    typedef typename Predicate::placeholders type;
  };
  
  template<class Dialect, class State, class Data, class Subdialect>
  struct select_statement :
    select_result_if<Data, typename Subdialect::exprs>,
    tag_if<fusion::result_of::has_key<Data, typename Subdialect::from>, select_statement_tag>
    {

    select_statement(const Data& data) : select_result_if<Data, typename Subdialect::exprs>(data) { }

    void str(std::ostream& os) const {
      os << "select";
      fusion::for_each(this->data_, str_clause(os));
    }

    template<class K, class T, class D = Data>
    struct transition {
      typedef select_statement<
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
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_begin_call.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_distinct.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_all.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_from.hpp>
    #include BOOST_PP_ITERATE()
    
    #include "detail/select_where.hpp"

    typedef typename placeholders_from_pair_list<Data>::type placeholders;
  };

  template<class Dialect, class State, class Data, class Subdialect>
  struct tag_of< select_statement<Dialect, State, Data, Subdialect> > {
    typedef typename select_statement<Dialect, State, Data, Subdialect>::tag type;
  };

} } }

#endif
