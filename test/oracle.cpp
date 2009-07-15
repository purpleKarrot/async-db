/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#include <boost/asio.hpp>
#include <boost/sql/oracle.hpp>
#include <iostream>

namespace db = boost::sql::oracle;

int main()
{
	try
	{
		boost::asio::io_service ios;
		db::connection conn(ios);

		conn.open("testdb", "root", "password", 0);

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
