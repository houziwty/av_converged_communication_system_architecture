//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 异步监听器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_IO_LISTEN_H
#define MODULE_NETWORK_ASIO_IO_LISTEN_H

#include "boost/asio.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/noncopyable.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class IoListen 
				: protected boost::noncopyable
			{
			public:
				IoListen(
					boost::asio::io_context& io,
					const unsigned short port = 10000);
				~IoListen(void);

			public:
				boost::asio::ip::tcp::acceptor& getListener(void);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
			};//class IoListen

			using IoListenPtr = boost::shared_ptr<IoListen>;
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_IO_LISTEN_H
