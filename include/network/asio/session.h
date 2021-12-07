//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : 会话
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_SESSION_H

#include "boost/enable_shared_from_this.hpp"
#include "libasio/io_session.h"
using namespace module::network::asio;
using IoSessionPtr = boost::shared_ptr<IoSession>;

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class Session 
				: protected boost::enable_shared_from_this<Session>
			{
			public:
				//@sess : IO会话
				Session(IoSessionPtr sess);
				virtual ~Session(void);

			public:
				//创建
				//@bytes [in] : 缓存大小，默认1MB
				//@Return : 错误码
				virtual int createNew(const unsigned int bytes /*= 1048576*/);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);

				//发送
				//@data : 数据
				//@bytes : 大小
				//@Return : 错误码
				int send(const void* data = nullptr, const int bytes = 0);

				//接收
				//@Return : 错误码
				int receive(void);

			protected:
				//发送事件通知
				//@s [in] : 客户端socket
				//@e [in] : socket错误码
				virtual void fetchSentDataEventNotification(const int e = 0, const int bytes = 0) = 0;

				//接收事件通知
				//@e [in] : socket错误码
				//@data [in] : 数据
				//@bytes [in] : 接收字节数
				virtual void fetchReceivedDataEventNotification(const int e = 0, const void* data = nullptr, const int bytes = 0) = 0;

			protected:
				IoSessionPtr ioSessionPtr;
			};//class Session
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_SESSION_H
