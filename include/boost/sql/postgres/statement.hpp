/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_POSTGRES_STATEMENT_HPP
#define BOOST_SQL_POSTGRES_STATEMENT_HPP

#include <boost/sql/postgres/connection.hpp>
#include <boost/sql/postgres/detail/bind_param.hpp>
#include <boost/sql/postgres/detail/bind_result.hpp>
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
		param_count = mpl::size<Param>::value,
		result_count = mpl::size<Result>::value
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
		Oid types[param_count];

		mpl::for_each<Param>(bind_oid(types));

		boost::shared_ptr<PGresult> res(PQprepare(conn.implementation(),
				stmt.c_str(), query_str.c_str(), param_count, types), PQclear);

		if (!res)
			throw std::bad_alloc();

		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));

#ifndef NDEBUG
		{
			boost::shared_ptr<PGresult> res(PQdescribePrepared(
					conn.implementation(), stmt.c_str()), PQclear);

			if (!res)
				throw std::bad_alloc();

			BOOST_ASSERT(PQnparams(res.get()) == param_count);
			BOOST_ASSERT(PQnfields(res.get()) == result_count);
		}
#endif

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		char* values[param_count];
		int lengths[param_count];
		int formats[param_count];

		// TODO: values need to be in network byte order!!
		fusion::for_each(params, bind_param(values, lengths, formats));

		res.reset(PQexecPrepared(conn.implementation(), stmt.c_str(),
				param_count, values, lengths, formats, 1), PQclear);

		if (!res)
			throw std::bad_alloc();

		ExecStatusType status = PQresultStatus(res.get());
		if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));

		row_number = 0;
		total_rows = PQntuples(res.get());
	}

	bool fetch(Result& result)
	{
		if (row_number >= total_rows)
			return false;

		fusion::for_each(result, detail::bind_result(res.get(), row_number++));
		return true;
	}

private:
	boost::shared_ptr<PGresult> res;
	int row_number;
	int total_rows;

	detail::stmt_impl_type stmt;
	connection& conn;
	std::string query_str;
	bool prepared;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_STATEMENT_HPP*/
