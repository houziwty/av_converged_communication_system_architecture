//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : 异步连接器
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_ASYNC_CONNECTOR_H
#define MODULE_NETWORK_ASIO_ASYNC_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			//连接事件回调
			//@_1 : socket
			//@_2 : 错误码
			typedef boost::function<void(boost::asio::ip::tcp::socket&, const int)> AsyncConnectEventCallback;

			class AsyncConnector 
				: public boost::enable_shared_from_this<AsyncConnector>
			{
			public:
				AsyncConnector(
					AsyncConnectEventCallback callback = nullptr);
				~AsyncConnector(void);

			public:
				//连接
				//@io [in] : IO实例
				//@ip : 远程IP
				//@port : 远程端口号
				//@Return : 错误码
				int connect(
					boost::asio::io_context& io, 
					const char* ip = nullptr,
					const unsigned short port = 0);

			private:
				AsyncConnectEventCallback asyncConnectEventCallback;
			};//class AsyncConnector
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_ASYNC_CONNECTOR_H
