/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_POSTGRES_STATEMENT_HPP
#define BOOST_SQL_POSTGRES_STATEMENT_HPP

#include <boost/sql/postgres/connection.hpp>
#include <boost/sql/postgres/detail/bind_param.hpp>
#include <boost/sql/postgres/detail/stmt_impl_type.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/fusion/mpl.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{

template<typename Param, typename Result>
class statement
{
	enum
	{
		size = mpl::size<Param>::value
	};

public:
	statement(connection& c, const std::string& query) :
		conn(c), query_str(query), prepared(false)
	{
	}

	~statement()
	{
		PQclear(PQexec(conn.implementation(),
				std::string("deallocate " + stmt).c_str()));
	}

	void prepare()
	{
		Oid types[size];

		mpl::for_each<Param>(bind_oid(types));

		boost::shared_ptr<PGresult> res(PQprepare(conn.implementation(),
				stmt.c_str(), query_str.c_str(), size, types), PQclear);

		if (!res)
			throw std::bad_alloc();

		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		char* values[size];
		int lengths[size];
		int formats[size];

		fusion::for_each(params, bind_param(values, lengths, formats));

		boost::shared_ptr<PGresult> res(PQexecPrepared(conn.implementation(),
				stmt.c_str(), size, values, lengths, formats, 1), PQclear);

		if (!res)
			throw std::bad_alloc();

		ExecStatusType status = PQresultStatus(res.get());
		if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));
	}

private:
	detail::stmt_impl_type stmt;
	connection& conn;
	std::string query_str;
	bool prepared;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_STATEMENT_HPP*/
