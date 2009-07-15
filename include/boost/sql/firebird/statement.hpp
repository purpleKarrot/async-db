/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_DETAIL_FIREBIRD_STATEMENT_HPP
#define BOOST_SQL_DETAIL_FIREBIRD_STATEMENT_HPP

#include <boost/sql/firebird/connection.hpp>
#include <boost/fusion/mpl/size.hpp>
#include <cstdlib>

namespace boost
{
namespace sql
{
namespace firebird
{

template<typename Param>
class statement
{
	enum
	{
		param_count = mpl::size<Param>::value, //
		in_sqlda_size = XSQLDA_LENGTH(param_count)
	};

	char in_sqlda[in_sqlda_size];

public:
	statement(connection& conn, const std::string& query) :
		impl(0), query_str(query), prepared(false)
	{
	}

	~statement()
	{
	}

	void prepare()
	{
		//isc_dsql_allocate_statement(status_vector, &db1, &stmt);
		//isc_dsql_prepare(status_vector, &trans, &stmt, 0, str, 1, NULL);

		XSQLDA* sqlda = reinterpret_cast<XSQLDA*> (in_sqlda);
		sqlda->version = SQLDA_VERSION1;
		sqlda->sqln = in_sqlda_size;

		// optional: benötigte parameter von db ermitteln lassen
		//isc_dsql_describe_bind(status_vector, &stmt, 1, sqlda);

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		//isc_dsql_execute(status_vector, &trans, &stmt, 1, NULL);
	}

private:
	isc_stmt_handle impl;
	std::string query_str;
	bool prepared;
};

} // end namespace firebird
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_FIREBIRD_STATEMENT_HPP */
