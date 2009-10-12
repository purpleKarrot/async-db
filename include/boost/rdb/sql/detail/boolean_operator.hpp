
template<class Expr1, class Expr2>
struct BOOST_RDB_OPERATOR_CLASS : binary_operation<Expr1, Expr2, precedence_level::boolean> {

  BOOST_RDB_OPERATOR_CLASS(const Expr1& expr1, const Expr2& expr2) : expr1_(expr1), expr2_(expr2) { }

  typedef boolean sql_type;
  
  void str(std::ostream& os) const {
    this->write(os, expr1_, BOOST_RDB_OPERATOR_STRING, expr2_);
  }
  
  Expr1 expr1_;
  Expr2 expr2_;
};

template<class Expr1, class Expr2>
BOOST_CONCEPT_REQUIRES(
  ((BooleanExpression<Expr1>))
  ((BooleanExpression<Expr2>)),
  (expression< BOOST_RDB_OPERATOR_CLASS<Expr1, Expr2> >))
operator BOOST_RDB_OPERATOR(const expression<Expr1>& expr1, const expression<Expr2>& expr2) {
  return expression< BOOST_RDB_OPERATOR_CLASS<Expr1, Expr2> >(expr1, expr2);
}

#undef BOOST_RDB_OPERATOR
#undef BOOST_RDB_OPERATOR_STRING
#undef BOOST_RDB_OPERATOR_CLASS
