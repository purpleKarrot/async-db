/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_DETAIL_THROW_ERROR_HPP
#define BOOST_SQL_ODBC_DETAIL_THROW_ERROR_HPP

#include <sql.h>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace boost
{
namespace sql
{
namespace odbc
{
namespace detail
{

inline void throw_error(SQLSMALLINT type, SQLHANDLE handle)
{
	SQLINTEGER i = 1;
	SQLINTEGER native;
	SQLCHAR state[6];
	std::vector<SQLCHAR> text(256);
	SQLSMALLINT len;
	SQLRETURN ret;
	std::stringstream stream;

	do
	{
		ret = SQLGetDiagRec(type, handle, i, state, &native, &text[0],
				text.size(), &len);

		if (ret == SQL_SUCCESS_WITH_INFO)
		{
			text.resize(len);
			continue;
		}
		if (ret == SQL_SUCCESS)
		{
			stream << (char*) state << ":" << i << ":" << native << ":"
					<< (char*) &text[0] << std::endl;
		}
		++i;
	} while (SQL_SUCCEEDED(ret));

	throw std::runtime_error(stream.str());
}

} // end namespace detail
} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_DETAIL_THROW_ERROR_HPP */
