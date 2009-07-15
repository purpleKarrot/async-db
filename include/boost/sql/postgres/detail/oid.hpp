/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_OID_HPP
#define BOOST_SQL_POSTGRES_OID_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <postgres_ext.h>
#include <catalog/pg_type.h>

namespace boost
{
namespace sql
{
namespace postgres
{
namespace detail
{

template<Oid OID>
struct oid_base
{
    BOOST_STATIC_CONSTANT(Oid, value = OID);
};

template<typename T>
struct oid
{
};

#define BOOST_SQL_DECLARE_OID(TYPE, OID) \
template<>struct oid<TYPE>: oid_base<OID> {};

BOOST_SQL_DECLARE_OID(bool, BOOLOID)
BOOST_SQL_DECLARE_OID(int16_t, INT2OID)
BOOST_SQL_DECLARE_OID(int32_t, INT4OID)
BOOST_SQL_DECLARE_OID(int64_t, INT8OID)
BOOST_SQL_DECLARE_OID(float, FLOAT4OID)
BOOST_SQL_DECLARE_OID(double, FLOAT8OID)
BOOST_SQL_DECLARE_OID(std::string, BYTEAOID)

#undef BOOST_SQL_DECLARE_OID

} // end namespace detail
} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_OID_HPP*/
