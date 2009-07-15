/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_TEST_EMPLOYEE_HPP
#define BOOST_SQL_TEST_EMPLOYEE_HPP

#include <string>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

struct Employee
{
	int id;
	std::string name;
	int salary;
	boost::optional<std::string> email;
	boost::optional<boost::gregorian::date> birthday;
};

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
		Employee,
		(int, id)
		(std::string, name)
		(int, salary)
		(boost::optional<std::string>, email)
		(boost::optional<boost::gregorian::date>, birthday)
)

#endif /* BOOST_SQL_TEST_EMPLOYEE_HPP */
