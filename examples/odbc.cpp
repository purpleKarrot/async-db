/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/sql/odbc.hpp>
#include <boost/fusion/tuple.hpp>

namespace db = boost::sql::odbc;

typedef boost::fusion::tuple<int, std::string, int> employee;
typedef boost::fusion::tuple<> nil;

struct Employees: db::connection
{
	Employees(boost::asio::io_service& ios) :
		db::connection(ios), insert(*this,
				"INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)"),
				count(*this, "SELECT count(*) FROM employee"), select(*this,
						"SELECT * FROM employee WHERE id=?")
	{
		open("DSN=MyTest;UID=root;PWD=password");

		std::cout << std::hex;
		std::cout << "client version: 0x" << client_version() << std::endl;
		std::cout << "server version: 0x" << server_version() << std::endl;
		std::cout << std::dec;
	}

	db::executable<void(int, std::string, int)> insert;
	db::executable<int()> count;
	db::executable<employee(int)> select;
};

int main()
{
	try
	{
		boost::asio::io_service ios;
		Employees empl(ios);

		db::statement<nil, nil> drop(empl, "DROP TABLE employee");
		db::statement<nil, nil> create(empl, "CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");

		drop.execute(nil());
		create.execute(nil());

		empl.insert(1002, "Horst", 712);
		empl.insert(1003, "Alfred", 732);

		std::cout << empl.count() << std::endl;
		std::cout << empl.select(1002) << std::endl;

		db::statement<nil, employee> get_all(empl, "SELECT * FROM employee");
		get_all.execute(nil());
		employee e;
		while (get_all.fetch(e))
			std::cout << e << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
