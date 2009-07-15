/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_DB_IMPL_BUFFER_HPP
#define BOOST_DB_IMPL_BUFFER_HPP

#include <string>

namespace boost
{
namespace sql
{

template<typename T>
const void * buffer( T & value )
{
	return &value;
}

const void * buffer( const std::string & str )
{
	return str.c_str();
}

const void * buffer( const char * str )
{
	return str;
}

} // end namespace sql
} // end namespace boost

#endif /*BOOST_DB_IMPL_BUFFER_HPP*/
