//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_DELETE_HPP
#define BOOST_RDB_DELETE_HPP

#include <boost/rdb/sql/common.hpp>

namespace boost { namespace rdb { namespace sql {

  struct delete_statement_tag : statement_tag { };

  template<class Predicate>
  struct delete_placeholders {
    typedef typename Predicate::placeholders placeholders;
  };

  template<>
  struct delete_placeholders<detail::none> {
    typedef fusion::vector<> placeholders;
  };

  // I should make this a FSM too someday...

  template<class Table, class Predicate>
  struct delete_statement : delete_placeholders<Predicate> {

    typedef delete_statement_tag tag;
    typedef void result;

    delete_statement(const Predicate& where) : where_(where) { }

    Predicate where_;

    void str(std::ostream& os) const { str(os, boost::is_same<Predicate, detail::none>()); }
    
    void str(std::ostream& os, boost::true_type) const {
      os << "delete from " << Table::table();
    }

    void str(std::ostream& os, boost::false_type) const {
      os << "delete from " << Table::table();
      os << " where ";
      where_.str(os);
    }

    template<class Where>
    delete_statement<Table, Where>
    where(const Where& where) const {
      BOOST_MPL_ASSERT((boost::is_same<Predicate, detail::none>));
      return delete_statement<Table, Where>(where);
    }
  };

  template<class Table>
  delete_statement<Table, detail::none>
  delete_from(const Table& table) {
    return delete_statement<Table, detail::none>(detail::none());
  }

} } }

#endif
