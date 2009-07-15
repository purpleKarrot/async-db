/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_POSTGRES_TRAITS_HPP
#define BOOST_SQL_POSTGRES_TRAITS_HPP

#include <postgres.h>
#include <postgres_ext.h>
#include <catalog/pg_type.h>

#include <boost/cstdint.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{

template<Oid OID, int Format = 1>
struct traits_base
{
	static Oid const oid = OID;
	static int const format = Format;
};

template<typename T>
struct traits: traits_base<InvalidOid>
{
};

template<>
struct traits<bool> : traits_base<BOOLOID>
{
};

template<>
struct traits<int64_t> : traits_base<INT8OID>
{
};

template<>
struct traits<int32_t> : traits_base<INT4OID>
{
};

template<>
struct traits<int16_t> : traits_base<INT2OID>
{
};

template<>
struct traits<double> : traits_base<FLOAT8OID>
{
};

template<>
struct traits<float> : traits_base<FLOAT4OID>
{
};

template<>
struct traits<std::string> : traits_base<BYTEAOID, 0>
{
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_TRAITS_HPP*/
