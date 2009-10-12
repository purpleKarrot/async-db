//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_TABLE_HPP
#define BOOST_RDB_TABLE_HPP

#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/sql/expression.hpp>

namespace boost { namespace rdb { namespace sql {

  struct any_table : boost::noncopyable {
    any_table() { }
    any_table(const std::string& alias) : alias_(alias) { }
    std::string alias_;
    const std::string& alias() const { return alias_; }
    bool has_alias() const { return !alias_.empty(); }
  };
  
  struct any_column /*: boost::noncopyable*/ {
    const any_table* table_;
    enum { precedence = precedence_level::highest };

    void initialize(const any_table* table) {
      table_ = table;
    }

    typedef fusion::vector<> placeholders;
  };

  template<class Table, class SqlType, class Base>
  struct column : Base {
    enum { precedence = precedence_level::highest };
    typedef SqlType sql_type;
    typedef Table table_type;
    typedef typename sql_type::cpp_type cpp_type;
    static void str_type(std::ostream& os) { SqlType::str(os); }
    
    template<class T>
    set_clause<column, typename result_of::make_expression<column, T>::type>
    operator =(const T& expr) const {
      return set_clause<column, typename result_of::make_expression<column, T>::type>(
        *this, expression<column>::make_expression(expr));
    }

    void str(std::ostream& os) const {
      if (this->table_->has_alias())
        os << this->table_->alias() << '.' << Base::name();
      else
        os << Base::name();
    }
  };

  template<class Col>
  struct Column
  {
    Col col;
    std::ostream& os;
    typedef typename Col::table_type table_type;

    BOOST_CONCEPT_USAGE(Column) {
      col.str(os);
    }
  };

  template<class Table>
  struct initialize_columns {
    initialize_columns(Table* pt) : pt(pt) { }
    template<typename T> void operator ()(T) {
      T::initialize(pt);
    }
    Table* pt;
  };

  template<typename T>
  struct singleton {
    static T _;
  };

  template<class T>
  T singleton<T>::_;
  
  template<class Base, bool IsSelfQualified>
  struct table_;
  
  template<class Base>
  struct table_<Base, false> : Base, any_table {
    table_() { }
    table_(const std::string& alias) : any_table(alias) { }
    
    void str(std::ostream& os) const {
      if (has_alias())
        os << Base::name() << " as " << alias_;
      else
        os << Base::name();
    }

    // following function name chosen because it won't conflict with column names :-P
    static const char* table() { return Base::name(); }
  };
  
  template<class Base>
  struct table_<Base, true> : Base, any_table {

    table_() : any_table(Base::name()) { }
    
    void str(std::ostream& os) const {
      os << Base::name();
    }
  };
  
  #define BOOST_RDB_BEGIN_TABLE(NAME)  \
  struct NAME##_base { static const char* name() { return #NAME; } }; \
  template<int Alias>  \
  struct NAME##_ : table_<NAME##_base, Alias == -1>, singleton< NAME##_<Alias> > {  \
    typedef NAME##_<Alias> this_table;  \
    typedef NAME##_<1> _1; typedef NAME##_<2> _2; typedef NAME##_<3> _3;  \
    NAME##_() { initialize(); }  \
    NAME##_(const std::string& alias) : table_<NAME##_base, Alias == -1>(alias) { initialize(); }  \
    NAME##_(const this_table& other) { initialize(); }  \
    typedef NAME##_<-1> qualified;  \
    typedef boost::mpl::vector0<>

  #define BOOST_RDB_END_TABLE(NAME)  \
    column_members; \
    void initialize() { \
      boost::mpl::for_each<this_table::column_members>(initialize_columns<this_table>(this)); \
    } \
  }; \
  typedef NAME##_<0> NAME;

  #define BOOST_RDB_COLUMN(NAME, sql_type) \
  members_before_##NAME;  \
  enum { NAME##_index = boost::mpl::size<members_before_##NAME>::value }; \
  struct NAME##_base : any_column { static const char* name() { return #NAME; } }; \
  typedef expression< column<this_table, sql_type, NAME##_base> > NAME##_type;  \
  NAME##_type NAME;  \
  struct NAME##_member {  \
    typedef std::string type;  \
    static NAME##_type& ref(this_table& obj) { return obj.NAME; }  \
    static const NAME##_type& ref(const this_table& obj) { return obj.NAME; }  \
    static void initialize(this_table* table) { table->NAME.initialize(table); }  \
  };  \
  typedef typename boost::mpl::push_back<members_before_##NAME, NAME##_member>::type

  template<typename Table>
  struct table_column_output : comma_output {
    table_column_output(std::ostream& os, const Table& table) : comma_output(os), table_(table) { }
    
    template<typename Column> void operator ()(Column) {
      std::ostream& os = item();
      os << Column::ref(table_).name() << " ";
      Column::ref(table_).str_type(os);
    }
    
    const Table& table_;
  };

  struct create_table_statement_tag : statement_tag { };

  template<typename Table>
  struct create_table_statement {
    typedef create_table_statement_tag tag;
    typedef void result;
    void str(std::ostream& os) const {
      os << "create table " << Table::table() << "(";
      boost::mpl::for_each<typename Table::column_members>(table_column_output<Table>(os, Table::_));
      os << ")";
    }
    std::string str() const { return as_string(*this); }
  };

  template<class Table>
  create_table_statement<Table> create_table(const Table&) {
    return create_table_statement<Table>();
  }

  struct drop_table_statement_tag : statement_tag { };

  template<typename Table>
  struct drop_table_statement {
    typedef drop_table_statement_tag tag;
    typedef void result;
    void str(std::ostream& os) const {
      os << "drop table " << Table::table();
    }
  };

  template<class Table>
  drop_table_statement<Table> drop_table(const Table&) {
    return drop_table_statement<Table>();
  }

} } }

#endif
