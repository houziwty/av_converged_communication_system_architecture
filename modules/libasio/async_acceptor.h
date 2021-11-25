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

#ifndef MODULE_NETWORK_ASIO_ASYNC_ACCEPTOR_H
#define MODULE_NETWORK_ASIO_ASYNC_ACCEPTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			//监听事件回调
			//@_1 : 客户端socket
			//@_2 : 错误码
			typedef boost::function<void(boost::asio::ip::tcp::socket&, const int)> AsyncAcceptEventCallback;

			class AsyncAcceptor 
				: public boost::enable_shared_from_this<AsyncAcceptor>
			{
			public:
				AsyncAcceptor(
					boost::asio::io_context& io,
					const unsigned short port = 10000,
					AsyncAcceptEventCallback callback = nullptr);
				~AsyncAcceptor(void);

			public:
				//监听
				//@Return : 错误码
				int listen(void);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				AsyncAcceptEventCallback asyncAcceptEventCallback;
			};//class AsyncAcceptor

			using AsyncAcceptorPtr = boost::shared_ptr<AsyncAcceptor>;
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_ASYNC_ACCEPTOR_H
