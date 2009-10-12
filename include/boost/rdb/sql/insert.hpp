//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_INSERT_HPP
#define BOOST_RDB_INSERT_HPP

#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/sql/expression.hpp>

namespace boost { namespace rdb { namespace sql {

  template<class Table>
  inline void str(std::ostream& os, const fusion::pair<sql2003::insert, const Table*>& p) {
    os << "insert into ";
    os << p.second->table();
  }

  template<class ColList>
  inline void str(std::ostream& os, const fusion::pair<sql2003::cols, ColList>& p) {
    os << " (";
    fusion::for_each(p.second, comma_output(os));
    os << ")";
  }

  template<class ValueList>
  inline void str(std::ostream& os, const fusion::pair<sql2003::values, ValueList>& p) {
    os << " values (";
    fusion::for_each(p.second, comma_output(os));
    os << ")";
  }

  template<class ExprList>
  inline void str(std::ostream& os, const fusion::pair<sql2003::select, ExprList>& p) {
    os << " select ";
    fusion::for_each(p.second, comma_output(os));
  }

  template<class Data, class HasValues, class HasSelect, class Subdialect>
  struct insert_impl {
  };

  struct extract_insert_values_placeholders {

    template<typename Sig>
    struct result;

    template<class Self, class Col, class Expr, class Placeholders>
    struct result<Self(fusion::vector<Col&, Expr&>, Placeholders&)> {
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

  template<class Data, class Subdialect>
  struct insert_impl<Data, mpl::true_, mpl::false_, Subdialect> {
    typedef insert_statement_tag tag;
    typedef typename fusion::result_of::value_at_key<Data, typename Subdialect::cols>::type cols;
    typedef typename fusion::result_of::value_at_key<Data, typename Subdialect::values>::type values;
    typedef fusion::zip_view< fusion::vector<cols&, values&> > zip_view;
    typedef typename fusion::result_of::as_vector<
      typename fusion::result_of::accumulate<zip_view, fusion::vector<>, extract_insert_values_placeholders>::type
    >::type placeholders;
  };

  template<class Data, class Subdialect>
  struct insert_impl<Data, mpl::false_, mpl::true_, Subdialect> {
    typedef insert_statement_tag tag;
    typedef typename placeholders_from_pair_list<Data>::type placeholders;
  };

  template<class Dialect, class State, class Data, class Subdialect>
  struct insert_statement : insert_impl<Data,
    typename fusion::result_of::has_key<Data, typename Subdialect::values>::type,
    typename fusion::result_of::has_key<Data, typename Subdialect::select>::type,
    Subdialect
  > {

    explicit insert_statement(const Data& data) : data_(data) { }

    typedef void result;

    Data data_;

    template<class K, class T, class D = Data>
    struct transition {
      typedef insert_statement<
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
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_cols.hpp>
    #include BOOST_PP_ITERATE()

    template<class ColIter, class ColLast, class ExprIter>
    struct value_list;

    template<class ColLast, class ExprIter>
    struct value_list<ColLast, ColLast, ExprIter> {
      typedef fusion::result_of::make_vector<>::type type;

      template<class T>
      static const fusion::vector<> make(const T&) {
        return fusion::vector<>();
      }
    };  

    template<class ColIter, class ColLast, class ExprIter>
    struct value_list {

      typedef typename remove_reference<
        typename fusion::result_of::deref<ColIter>::type
      >::type col_type;

      typedef value_list<
        typename fusion::result_of::next<ColIter>::type,
        ColLast,
        typename fusion::result_of::next<ExprIter>::type
      > next;

      typedef typename fusion::result_of::push_front<
        const typename next::type,
        typename result_of::make_expression<
          col_type,
          typename remove_reference<
            typename fusion::result_of::deref<ExprIter>::type
          >::type
        >::type
      >::type type;

      template<class T>
      static const type make(const T& iter) { // why doesn't ExprIter work ?
        return fusion::push_front(
          next::make(fusion::next(iter)),
          expression<col_type>::make_expression(*iter)
          );
      }
    };  

    template<class Exprs>
    struct with_values {

      typedef typename fusion::result_of::value_at_key<Data, typename Subdialect::cols>::type cols;
      typedef typename fusion::result_of::end<cols>::type col_last;

      // If this assertion fails the insert list and the value list have different sizes
      BOOST_MPL_ASSERT((mpl::equal_to<
        fusion::result_of::size<cols>,
        fusion::result_of::size<Exprs> >));
      //BOOST_MPL_ASSERT((is_sql_compatible<cols, Exprs>));

      typedef value_list<
        typename fusion::result_of::begin<cols>::type,
        typename fusion::result_of::end<cols>::type,
        typename fusion::result_of::begin<Exprs>::type
      > final_value_list;

      typedef typename transition<
        typename Subdialect::values,
        typename fusion::result_of::as_vector<
          typename final_value_list::type
        >::type
      >::type type;

      static type make(const Data& data, const Exprs& exprs) {
        return type(add_key<typename Subdialect::values>(
          data, fusion::as_vector(final_value_list::make(fusion::begin(exprs)))));
      }
    };

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_values.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_select.hpp>
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

    void str(std::ostream& os) const {
      fusion::for_each(data_, str_clause(os));
    }

  };
  
  BOOST_RDB_ALLOW(sql2003, cols, select);

  template<class Table>
  insert_statement<
    sql2003,
    sql2003::insert,
    fusion::map<
      fusion::pair<
      sql2003::insert, const Table*
      >
    >,
    sql2003
  >
  insert_into(const Table& table) {
    return insert_statement<
      sql2003,
      sql2003::insert,
      fusion::map<
        fusion::pair<
        sql2003::insert, const Table*
        >
      >,
      sql2003
    >(fusion::make_pair<sql2003::insert>(&table));
  }

} } }

#endif
