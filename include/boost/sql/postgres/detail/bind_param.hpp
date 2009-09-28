/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_BIND_PARAM_HPP
#define BOOST_SQL_POSTGRES_BIND_PARAM_HPP

#include <boost/sql/detail/buffer.hpp>
#include <boost/sql/detail/buffer_length.hpp>
#include <boost/sql/postgres/detail/traits.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{

class bind_oid
{
public:
	bind_oid(Oid* type) :
		type_(type)
	{
	}

	template<typename T>
	void operator()(T) const
	{
		*type_++ = traits<T>::oid;
	}

private:
	mutable Oid* type_;
};

class bind_param
{
public:
	bind_param(char** value, int* length, int* format) :
		value_(value), length_(length), format_(format)
	{
	}

	template<typename T>
	void operator()(const T& value) const
	{
		//TODO: value needs to be in network byte order!
		*value_++ = (char*) (buffer(value));
		*length_++ = buffer_length(value);
		*format_++ = traits<T>::format;
	}

private:
	mutable char** value_;
	mutable int* length_;
	mutable int* format_;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_BIND_PARAM_HPP*/
