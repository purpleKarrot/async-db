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

typedef boost::fusion::tuple<int, std::string, int> employee;
typedef boost::fusion::tuple<> nil;

struct Employees: db::connection
{
	Employees(boost::asio::io_service& ios) :
		db::connection(ios)
	{
		open("dbname=foo user=postgres password=postgres");

		insert = prepare<void(int, std::string, int)> (
				"INSERT INTO employee (id, name, salary) VALUES ($1, $2, $3)");
		count = prepare<int()> ("SELECT count(*) FROM employee");
		select = prepare<employee(int)> ("SELECT * FROM employee WHERE id=$1");

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	boost::function<void(int, std::string, int)> insert;
	boost::function<int()> count;
	boost::function<employee(int)> select;
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

		std::cout << empl.count() << std::endl;
		std::cout << empl.select(1002) << std::endl;

		db::statement<nil, employee> get_all(empl, "SELECT * FROM employee");
		get_all.execute(nil());
		employee e;
		while (get_all.fetch(e))
			std::cout << e << std::endl;

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
