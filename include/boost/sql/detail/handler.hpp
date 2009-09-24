/**************************************************************
 * Copyright (c) 2008-2009 Daniel Pfeifer                     *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/

#ifndef BOOST_SQL_DETAIL_HANDLER_HPP
#define BOOST_SQL_DETAIL_HANDLER_HPP

#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>

namespace boost
{
namespace sql
{
namespace detail
{

template<typename Function, typename Callback>
class handler
{
public:
	handler(boost::asio::io_service& io_service, //
			Function function, Callback callback) :
		io_service_(io_service), work_(io_service), //
				function_(function), callback_(callback)
	{
	}

	void operator()()
	{
		boost::system::error_code error;
		function_(error);
		io_service_.post(boost::bind(callback_, error));
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::io_service::work work_;
	Function function_;
	Callback callback_;
};

} // namespace detail
} // namespace sql
} // namespace boost

#endif /* BOOST_SQL_DETAIL_HANDLER_HPP */
