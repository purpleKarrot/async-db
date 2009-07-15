/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_CONNECTION_BASE_HPP
#define BOOST_SQL_DETAIL_CONNECTION_BASE_HPP

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>

namespace boost
{
namespace sql
{
namespace detail
{

template<typename Service>
class connection_base: private noncopyable
{
public:
	/// The type of the service that will be used to provide I/O operations.
	typedef Service service_type;

	/// Get the io_service associated with the object.
	/**
	 * This function may be used to obtain the io_service object that the I/O
	 * object uses to dispatch handlers for asynchronous operations.
	 *
	 * @return A reference to the io_service object that the I/O object will use
	 * to dispatch handlers. Ownership is not transferred to the caller.
	 */
	asio::io_service& get_io_service()
	{
		return service.get_io_service();
	}

protected:
	/// Construct a basic_io_object.
	explicit connection_base(asio::io_service& io_service) :
		service(asio::use_service<Service>(io_service))
	{
	}

	/// Protected destructor to prevent deletion through this type.
	~connection_base()
	{
	}

	/// The backend service implementation.
	service_type& service;
};

} // end namespace detail
} // end namespace sql
} // end namespace boost

#endif // BOOST_SQL_DETAIL_CONNECTION_BASE_HPP
