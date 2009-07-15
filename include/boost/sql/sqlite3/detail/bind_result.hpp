/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_SQLITE3_DETAIL_BIND_RESULT_HPP
#define BOOST_SQL_SQLITE3_DETAIL_BIND_RESULT_HPP

#include <sqlite3.h>
#include <boost/optional.hpp>
#include <string>

namespace boost
{
namespace sql
{
namespace sqlite3
{
namespace detail
{

class bind_result
{
public:
	bind_result(sqlite3_stmt* stmt) :
		stmt(stmt), pos(0)
	{
	}

	void operator()(int& value) const
	{
		value = sqlite3_column_int(stmt, pos++);
	}

	void operator()(sqlite_int64& value) const
	{
		value = sqlite3_column_int64(stmt, pos++);
	}

	void operator()(std::string& value) const
	{
		value.assign(reinterpret_cast<const char*> (sqlite3_column_text(stmt,
				pos)),
				static_cast<std::size_t> (sqlite3_column_bytes(stmt, pos)));
		++pos;
	}

//	template<typename T>
//	void operator()(boost::optional<T>& value) const
//	{
//		if (sqlite3_column_type(stmt, pos) != SQLITE_NULL)
//		{
//			operator()(*value);
//		}
//		else
//		{
//			value = boost::optional<T>();
//			++pos;
//		}
//	}

private:
	sqlite3_stmt* stmt;
	mutable int pos;
};

} // end namespace detail
} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_SQLITE3_DETAIL_BIND_RESULT_HPP */
