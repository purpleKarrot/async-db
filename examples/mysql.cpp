/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/sql/mysql.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/function.hpp>

namespace db = boost::sql::mysql;

typedef boost::fusion::tuple<int, std::string, boost::gregorian::date> user;

typedef boost::fusion::tuple<> nil;

struct Users: db::connection
{
	Users(boost::asio::io_service& io_service) :
		db::connection(io_service)
	{
		open("testdb", "root", "password");

		insert = prepare<void(int, std::string, boost::gregorian::date)> (
				"INSERT INTO users (id, name, registered) VALUES (?, ?, ?)");
		count = prepare<int()>("SELECT count(*) FROM users");
		select = prepare<user(int)> ("SELECT * FROM users WHERE id=?");

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	boost::function<void(int, std::string, boost::gregorian::date)> insert;
	boost::function<int()> count;
	boost::function<user(int)> select;
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
		boost::asio::io_service io_service;
		Users users(io_service);

		db::statement<nil, nil> drop(users, "DROP TABLE users");
		db::statement<nil, nil> create(users, "CREATE TABLE users ( "
			"id INT NOT NULL, name CHAR(20) NOT NULL, registered DATE,"
			"PRIMARY KEY (id) )");

		drop.async_execute(nil(), drop_handler);
		create.async_execute(nil(), create_handler);

		io_service.run();

		users.execute("INSERT INTO users (id, name, registered) "
			"VALUES (1001, 'John', NOW())");

		users.insert(1002, "Carl", boost::gregorian::from_string("2002/1/25"));
		users.insert(1003, "Bob", boost::gregorian::day_clock::local_day());

		std::cout << "\nNumber of users: " << users.count() << std::endl;
		std::cout << "User with id=1002: " << users.select(1002) << std::endl;

		std::cout << "\nAll users:" << std::endl;
		db::statement<nil, user> get_all(users, "SELECT * FROM users");
		get_all.execute(nil());
		user u;
		while (get_all.fetch(u))
			std::cout << u << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
}
