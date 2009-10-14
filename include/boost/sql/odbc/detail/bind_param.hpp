/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_DETAIL_BIND_PARAM_HPP
#define BOOST_SQL_ODBC_DETAIL_BIND_PARAM_HPP

#include <sql.h>
#include <boost/sql/odbc/detail/traits.hpp>
#include <boost/sql/odbc/detail/throw_error.hpp>
#include <boost/sql/detail/buffer.hpp>
#include <boost/sql/detail/buffer_length.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{
namespace detail
{

class bind_param
{
public:
	bind_param(HSTMT hstmt, SQLLEN* indicator) :
		hstmt_(hstmt), pos_(1), indicator_(indicator)
	{
	}

	template<typename T>
	void operator()(const T& value) const
	{
		*indicator_ = buffer_length(value);

		if (!SQL_SUCCEEDED(SQLBindParameter(hstmt_, pos_, SQL_PARAM_INPUT,
						traits<T>::value_type, traits<T>::param_type, 0, 0,
						const_cast<void*> (buffer(value)), 0, indicator_)))
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

#endif /* BOOST_SQL_ODBC_DETAIL_BIND_PARAM_HPP */
