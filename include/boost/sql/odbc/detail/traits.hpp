/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ODBC_DETAIL_TRAITS_HPP
#define BOOST_SQL_ODBC_DETAIL_TRAITS_HPP

#include <sql.h>
#include <sqlext.h>
#include <string>
#include <boost/cstdint.hpp>

namespace boost
{
namespace sql
{
namespace odbc
{
namespace detail
{

template<SQLSMALLINT ValueType, SQLSMALLINT ParamType>
struct traits_base
{
	static SQLSMALLINT const value_type = ValueType;
	static SQLSMALLINT const param_type = ParamType;
};

template<typename T>
struct traits: traits_base<SQL_C_DEFAULT, SQL_VARCHAR>
{
};

#define BOOST_ODBC_TRAITS(type, sql_c, sql) \
	template<> struct traits<type> : traits_base<sql_c, sql> {};

BOOST_ODBC_TRAITS(int8_t, SQL_C_STINYINT, SQL_TINYINT)
BOOST_ODBC_TRAITS(uint8_t, SQL_C_UTINYINT, SQL_TINYINT)
BOOST_ODBC_TRAITS(int16_t, SQL_C_SSHORT, SQL_SMALLINT)
BOOST_ODBC_TRAITS(uint16_t, SQL_C_USHORT, SQL_SMALLINT)
BOOST_ODBC_TRAITS(int32_t, SQL_C_SLONG, SQL_INTEGER)
BOOST_ODBC_TRAITS(uint32_t, SQL_C_ULONG, SQL_INTEGER)
BOOST_ODBC_TRAITS(int64_t, SQL_C_SBIGINT, SQL_BIGINT)
BOOST_ODBC_TRAITS(uint64_t, SQL_C_UBIGINT, SQL_BIGINT)
BOOST_ODBC_TRAITS(float, SQL_C_FLOAT, SQL_FLOAT)
BOOST_ODBC_TRAITS(double, SQL_C_DOUBLE, SQL_DOUBLE)
BOOST_ODBC_TRAITS(std::string, SQL_C_CHAR, SQL_VARCHAR)
BOOST_ODBC_TRAITS(std::wstring, SQL_C_WCHAR, SQL_WVARCHAR)

#undef BOOST_ODBC_TRAITS

} // end namespace detail
} // end namespace odbc
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ODBC_DETAIL_TRAITS_HPP */
