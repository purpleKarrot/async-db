/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_DB_IMPL_BUFFER_LENGTH_HPP
#define BOOST_DB_IMPL_BUFFER_LENGTH_HPP

#include <string>
#include <cstring>

namespace boost
{
namespace sql
{

template<typename T>
unsigned long buffer_length(T&)
{
	return sizeof(T);
}

unsigned long buffer_length(const std::string& str)
{
	return str.length();
}

unsigned long buffer_length(const char* str)
{
	return std::strlen(str);
}

template<unsigned long N>
unsigned long buffer_length(const char (&)[N])
{
	return N;
}

} // end namespace sql
} // end namespace boost

#endif /*BOOST_DB_IMPL_BUFFER_LENGTH_HPP*/
