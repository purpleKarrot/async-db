/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_DETAIL_BIND_RESULT_HPP
#define BOOST_SQL_POSTGRES_DETAIL_BIND_RESULT_HPP

#include <boost/sql/postgres/detail/traits.hpp>
#include <boost/integer/endian.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{
namespace detail
{

class bind_result
{
public:
	bind_result(const PGresult *res, int row_number) :
		res_(res), row_(row_number), col_(0)
	{
	}

	template<typename T>
	void operator()(T& value) const
	{
		//BOOST_ASSERT(PQftype(res_, col_) == traits<T>::oid);

		set(value, PQgetvalue(res_, row_, col_), PQgetlength(res_, row_, col_),
				PQgetisnull(res_, row_, col_));
		++col_;
	}

private:
	void set(int& value, char* data, int length, int isnull) const
	{
		if (length == 8)
			value = *reinterpret_cast<boost::integer::big64_t*> (data);
		else if (length == 4)
			value = *reinterpret_cast<boost::integer::big32_t*> (data);
	}

	void set(std::string& value, char* data, int length, int isnull) const
	{
		value.assign(data, length);
	}

	//	template<typename T>
	//	void set(boost::optional<T>& value, char* data, int length, int isnull) const
	//	{
	//	}

private:
	const PGresult* res_;
	int row_;
	mutable int col_;
};

} // end namespace detail
} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_POSTGRES_DETAIL_BIND_RESULT_HPP */
