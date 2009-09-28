/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/sql/sqlite3.hpp>
#include <boost/fusion/tuple.hpp>

namespace db = boost::sql::sqlite3;

typedef boost::fusion::tuple<int, std::string, int> employee;
typedef boost::fusion::tuple<> nil;

struct Employees: db::connection
{
	Employees(boost::asio::io_service& ios) :
		db::connection(ios),
		insert(*this, "INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)"),
		count(*this, "SELECT count(*) FROM employee"),
		select(*this, "SELECT * FROM employee WHERE id=?")
	{
		open("sqlite3.db");

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	db::executable<void(int, std::string, int)> insert;
	db::executable<int()> count;
	db::executable<employee(int)> select;
};

void drop_handler(boost::system::error_code& error)
{
	std::cout << "drop_handler: " << error.message() << std::endl;
}

void create_handler(boost::system::error_code& error)
{
	std::cout << "create_handler: " << error.message() << std::endl;
}

int main()
{
	try
	{
		boost::asio::io_service ios;
		Employees empl(ios);

		db::statement<nil, nil> drop(empl, "DROP TABLE employee");
		drop.async_execute(nil(), drop_handler);

		db::statement<nil, nil> create(empl, "CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");
		create.async_execute(nil(), create_handler);

		ios.run();
		ios.reset();

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
