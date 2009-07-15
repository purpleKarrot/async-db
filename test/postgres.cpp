/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/sql/postgres.hpp>
#include <boost/fusion/tuple.hpp>

namespace db = boost::sql::postgres;

struct Employees: db::connection
{
	Employees(boost::asio::io_service& ios) :
		db::connection(ios),
		insert(*this, "INSERT INTO employee (id, name, salary)"
			" VALUES ($1, $2, $3)")
	{
		open("dbname=foo user=postgres password=postgres");

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	db::executable<void(int, std::string, int)> insert;
};

int main()
{
	try
	{
		boost::asio::io_service ios;
		Employees empl(ios);

		empl.execute("DROP TABLE employee");

		empl.execute("CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");

		empl.execute("INSERT INTO employee (id, name, salary) "
			"VALUES (1001, 'Thad Beaumont', 44000)");

		empl.insert(1002, "Horst", 712);
		empl.insert(1003, "Alfred", 7132);

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
