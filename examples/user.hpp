/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_TEST_USER_HPP
#define BOOST_SQL_TEST_USER_HPP

#include <string>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

struct User
{
	int id;
	std::string name;
	boost::gregorian::date registered;
	boost::optional<std::string> website;
};

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(User,
	(int, id)
	(std::string, name)
	(boost::gregorian::date, registered)
	(boost::optional<std::string>, website)
)

#endif /* BOOST_SQL_TEST_USER_HPP */
