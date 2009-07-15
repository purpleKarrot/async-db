/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_SERVICE_BASE_HPP
#define BOOST_SQL_DETAIL_SERVICE_BASE_HPP

#include <boost/asio/io_service.hpp>

namespace boost
{
namespace sql
{
namespace detail
{

template<typename Type>
class service_id: public asio::io_service::id
{
};

template<typename Type>
class service_base: public asio::io_service::service
{
public:
	static service_id<Type> id;

protected:
	service_base(asio::io_service& io_service) :
		asio::io_service::service(io_service)
	{
	}
};

template<typename Type>
service_id<Type> service_base<Type>::id;

} // end namespace detail
} // end namespace sql
} // end namespace boost

#endif /* BOOST_SQL_DETAIL_SERVICE_BASE_HPP */
