//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : 异步TCP服务端
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASYNC_TCP_SERVER_H
#define FRAMEWORK_NETWORK_ASYNC_TCP_SERVER_H

#include "boost/shared_ptr.hpp"
#include "libasio/io_service.h"
using namespace module::network::asio;
using IoServicePtr = boost::shared_ptr<IoService>;

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class AsyncTcpServer
			{
			public:
				AsyncTcpServer(void);
				virtual ~AsyncTcpServer(void);

			public:
				//创建
				//@port [in] : 监听端口号
				//@Return : 错误码
				virtual int createNew(const unsigned short port /*= 10000*/);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);

			protected:
				//监听事件通知
				//@s [in] : 客户端socket
				virtual void afterFetchAsyncAcceptEventNotification(boost::asio::ip::tcp::socket& s) = 0;

			private:
				//监听事件回调
				//@s [in] : 客户端socket
				//@e [in] : socket错误码
				void asyncAcceptEventCallback(boost::asio::ip::tcp::socket& s, const int e = 0);

			private:
				IoServicePtr ioServicePtr;
			};//class AsyncTcpServer
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASYNC_TCP_SERVER_H
