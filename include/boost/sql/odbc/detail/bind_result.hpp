/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_DETAIL_BIND_RESULT_HPP
#define BOOST_SQL_ODBC_DETAIL_BIND_RESULT_HPP

#include <sql.h>
#include <boost/sql/odbc/detail/traits.hpp>
#include <boost/sql/odbc/detail/throw_error.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{
namespace detail
{

class bind_result
{
public:
	bind_result(HSTMT hstmt, SQLINTEGER* indicator) :
		hstmt_(hstmt), pos_(1), indicator_(indicator)
	{
	}

	template<typename T>
	void operator()(const T& value) const
	{
		if (!SQL_SUCCEEDED(SQLBindCol(hstmt_, pos_, traits<T>::value_type,
						const_cast<void*> (buffer(value)),
						buffer_length(value), indicator_)))
		{
			detail::throw_error(SQL_HANDLE_STMT, hstmt_);
		}

		++pos_;
		++indicator_;
	}

private:
	HSTMT hstmt_;
	mutable int pos_;
	mutable SQLLEN* indicator_;
};

} // end namespace detail
} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_DETAIL_BIND_RESULT_HPP */
