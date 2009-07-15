/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_ORACLE_CONNECTION_HPP
#define BOOST_SQL_ORACLE_CONNECTION_HPP

#include <boost/sql/oracle/detail/service.hpp>
#include <boost/sql/detail/connection_base.hpp>

namespace boost
{
namespace sql
{
namespace oracle
{

class connection: sql::detail::connection_base<detail::service>
{
public:
	connection(asio::io_service& io_service) :
		sql::detail::connection_base<detail::service>(io_service)
	{
		if (OCIHandleAlloc(service.env(), reinterpret_cast<dvoid**> (&server),
				OCI_HTYPE_SERVER, 0, 0))
			throw std::bad_alloc();

		if (OCIHandleAlloc(service.env(), reinterpret_cast<dvoid**> (&error),
				OCI_HTYPE_ERROR, 0, 0))
			throw std::bad_alloc();

		if (OCIHandleAlloc(service.env(), reinterpret_cast<dvoid**> (&svcctx),
				OCI_HTYPE_SVCCTX, 0, 0))
			throw std::bad_alloc();

		if (OCIHandleAlloc(service.env(), reinterpret_cast<dvoid**> (&session),
				OCI_HTYPE_SESSION, 0, 0))
			throw std::bad_alloc();
	}

	~connection()
	{
		OCISessionEnd(svcctx, error, session, OCI_DEFAULT);
		OCIServerDetach(server, error, OCI_DEFAULT);

		OCIHandleFree(session, OCI_HTYPE_SESSION);
		OCIHandleFree(svcctx, OCI_HTYPE_SVCCTX);
		OCIHandleFree(error, OCI_HTYPE_ERROR);
		OCIHandleFree(server, OCI_HTYPE_SERVER);
	}

	void open(const std::string& service, const std::string& username,
			const std::string& password, int mode)
	{
		if (OCIServerAttach(server, error, (text*) service.c_str(),
				service.length(), OCI_DEFAULT))
			throw_error();

		if (OCIAttrSet(svcctx, OCI_HTYPE_SVCCTX, server, 0, OCI_ATTR_SERVER,
				error))
			throw_error();

		if (OCIAttrSet(session, OCI_HTYPE_SESSION, (dvoid*) username.c_str(),
				username.length(), OCI_ATTR_USERNAME, error))
			throw_error();

		if (OCIAttrSet(session, OCI_HTYPE_SESSION, (dvoid*) password.c_str(),
				password.length(), OCI_ATTR_PASSWORD, error))
			throw_error();

		if (OCISessionBegin(svcctx, error, session, OCI_CRED_RDBMS, mode))
			throw_error();

		if (OCIAttrSet(svcctx, OCI_HTYPE_SVCCTX, session, 0, OCI_ATTR_SESSION,
				error))
			throw_error();
	}

	unsigned long client_version()
	{
		return 0;
	}

	unsigned long server_version()
	{
		return 0;
	}

	void execute(const std::string& query)
	{
	}

private:
	void throw_error()
	{
		char errbuf[512];
		sb4 errcode;

		OCIErrorGet(error, 1, 0, &errcode, (text*) errbuf, sizeof(errbuf),
				OCI_HTYPE_ERROR);

		throw std::runtime_error(errbuf);
	}

	OCIServer* server;
	OCIError* error;
	OCISvcCtx* svcctx;
	OCISession* session;
};

} // end namespace oracle
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_ORACLE_CONNECTION_HPP */
