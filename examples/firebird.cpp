/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/sql/firebird.hpp>
//#include <boost/fusion/tuple.hpp>

namespace db = boost::sql::firebird;

const char parm_buffer[] =
{ isc_dpb_version1, isc_dpb_user_name, 6, 'S', 'Y', 'S', 'D', 'B', 'A',
		isc_dpb_password, 6, 'k', 'o', 'r', 'o', 'w', 'a', isc_dpb_lc_ctype,
		11, 'U', 'N', 'I', 'C', 'O', 'D', 'E', '_', 'F', 'S', 'S' };

struct Employees: db::connection
{
	Employees(boost::asio::io_service& io_service) :
		db::connection(io_service)
	/* insert(*this, "INSERT INTO employee (id, name, salary)"
	 " VALUES (?, ?, ?)")*/
	{
		open("firebird.fdb", parm_buffer);

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	//	db::statement<boost::fusion::tuple<int, std::string, int> > insert;
};

int main()
{
	try
	{
		boost::asio::io_service ios;
		Employees empl(ios);

		//		empl.execute("DROP TABLE employee");
		//
		//		empl.execute("CREATE TABLE employee"
		//			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");
		//
		//		empl.execute("INSERT INTO employee (id, name, salary) "
		//			"VALUES (1001, 'Thad Beaumont', 44000)");
		//
		//		empl.insert.execute(boost::fusion::make_tuple(1002, "Horst", 712));
		//		empl.insert(1003, "Alfred", 7132);

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
