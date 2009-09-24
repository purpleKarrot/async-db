/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_SQLITE3_DETAIL_ERROR_HPP
#define BOOST_SQL_SQLITE3_DETAIL_ERROR_HPP

#include <sqlite3.h>
#include <boost/system/system_error.hpp>

namespace boost
{
namespace sql
{
namespace sqlite3
{
namespace detail
{

struct error_category: boost::system::error_category
{
	const char* name() const
	{
		return "sqlite3";
	}

	std::string message(int value) const
	{
		if (value == SQLITE_OK)
			return "Successful result";
		if (value == SQLITE_ERROR)
			return "SQL error or missing database";
		if (value == SQLITE_INTERNAL)
			return "Internal logic error in SQLite";
		if (value == SQLITE_PERM)
			return "Access permission denied";
		if (value == SQLITE_ABORT)
			return "Callback routine requested an abort";
		if (value == SQLITE_BUSY)
			return "The database file is locked";
		if (value == SQLITE_LOCKED)
			return "A table in the database is locked";
		if (value == SQLITE_NOMEM)
			return "A malloc() failed";
		if (value == SQLITE_READONLY)
			return "Attempt to write a readonly database";
		if (value == SQLITE_INTERRUPT)
			return "Operation terminated by sqlite3_interrupt()";
		if (value == SQLITE_IOERR)
			return "Some kind of disk I/O error occurred";
		if (value == SQLITE_CORRUPT)
			return "The database disk image is malformed";
		//if (value == SQLITE_NOTFOUND) // NOT USED.
		//	return "Table or record not found";
		if (value == SQLITE_FULL)
			return "Insertion failed because database is full";
		if (value == SQLITE_CANTOPEN)
			return "Unable to open the database file";
		//if (value == SQLITE_PROTOCOL) // NOT USED.
		//	return "Database lock protocol error";
		if (value == SQLITE_EMPTY)
			return "Database is empty";
		if (value == SQLITE_SCHEMA)
			return "The database schema changed";
		if (value == SQLITE_TOOBIG)
			return "String or BLOB exceeds size limit";
		if (value == SQLITE_CONSTRAINT)
			return "Abort due to constraint violation";
		if (value == SQLITE_MISMATCH)
			return "Data type mismatch";
		if (value == SQLITE_MISUSE)
			return "Library used incorrectly";
		if (value == SQLITE_NOLFS)
			return "Uses OS features not supported on host";
		if (value == SQLITE_AUTH)
			return "Authorization denied";
		if (value == SQLITE_FORMAT)
			return "Auxiliary database format error";
		if (value == SQLITE_RANGE)
			return "2nd parameter to sqlite3_bind out of range";
		if (value == SQLITE_NOTADB)
			return "File opened that is not a database file";
		if (value == SQLITE_ROW)
			return "sqlite3_step() has another row ready";
		if (value == SQLITE_DONE)
			return "sqlite3_step() has finished executing";
		return "Unknown error";
	}
};

} // namespace detail

boost::system::error_category& get_error_category()
{
	static detail::error_category category;
	return category;
}

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_SQLITE3_DETAIL_ERROR_HPP */
