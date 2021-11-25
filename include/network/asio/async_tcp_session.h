//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : 异步TCP会话
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASYNC_TCP_SESSION_H
#define FRAMEWORK_NETWORK_ASYNC_TCP_SESSION_H

#include "boost/shared_ptr.hpp"
#include "libasio/io_session.h"
using namespace module::network::asio;
using IoSessionPtr = boost::shared_ptr<IoSession>;

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class AsyncTcpSession
			{
			public:
				AsyncTcpSession(boost::asio::ip::tcp::socket& s);
				virtual ~AsyncTcpSession(void);

			public:
				//创建
				//@bytes [in] : 缓存大小，默认1MB
				//@Return : 错误码
				virtual int createNew(const int bytes /*= 1048576*/);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);

				//发送
				//@data : 数据
				//@bytes : 大小
				//@Return : 错误码
				int send(const void* data = nullptr, const int bytes = 0);

				//接收
				//@s : socket
				//@Return : 错误码
				int receive(void);

			protected:
				//发送事件通知
				//@s [in] : 客户端socket
				//@e [in] : socket错误码
				virtual void afterFetchSendEventNotification(const int e = 0, const int bytes = 0) = 0;

				//接收事件通知
				//@e [in] : socket错误码
				//@data [in] : 数据
				//@bytes [in] : 接收字节数
				virtual void asyncFetchReceiveEventNotification(const int e = 0, const void* data = nullptr, const int bytes = 0) = 0;

			private:
				//发送事件回调
				//@e [in] : socket错误码
				//@bytes [in] : 发送字节数
				void asyncSendEventCallback(const int e = 0, const int bytes = 0);

				//接收事件回调
				//@e [in] : socket错误码
				//@data [in] : 数据
				//@bytes [in] : 接收字节数
				void asyncReceiveEventCallback(const int e = 0, const void* data = nullptr, const int bytes = 0);

			protected:
				boost::asio::ip::tcp::socket so;
				IoSessionPtr ioSessionPtr;
			};//class AsyncTcpSession

			using AsyncTcpSessionPtr = boost::shared_ptr<AsyncTcpSession>;
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASYNC_TCP_SESSION_H
