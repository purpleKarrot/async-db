
    template<class Predicate>
    typename transition<typename Subdialect::where, Predicate>::type
    where(const Predicate& predicate) const {
      BOOST_MPL_ASSERT((allow<Subdialect, State, typename Subdialect::where>));
      return typename transition<typename Subdialect::where, Predicate>::type(
        add_key<typename Subdialect::where>(this->data_, predicate));
    }
