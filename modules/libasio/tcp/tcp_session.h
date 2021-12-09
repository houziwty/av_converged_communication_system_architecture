//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : TCP会话
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_TCP_SESSION_H
#define MODULE_NETWORK_ASIO_TCP_SESSION_H

#include "libasio/defs.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class TcpSession
			{
			public:
				TcpSession(SessionPtr ptr);
				virtual ~TcpSession(void);

			public:
				//创建
				//@bytes [in] : 缓存大小，默认1MB
				//@Return : 错误码
				virtual int createNew(const unsigned int bytes = 1048576);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);
				
				//发送
				//@data : 数据
				//@bytes : 大小
				//@Return : 错误码
				int send(
					const void* data = nullptr,
					const int bytes = 0);

				//接收
				//@Return : 错误码
				int receive(void);

			protected:
				//发送事件通知
				//@e [out] : socket错误码
				//@bytes [out] : 大小
				virtual void fetchSentDataEventNotification(
					const int e = 0, 
					const int bytes = 0) = 0;

				//接收事件通知
				//@e [out] : socket错误码
				//@bytes [out] : 大小
				virtual void fetchReceivedDataEventNotification(
					const int e = 0, 
					const void* data = nullptr, 
					const int bytes = 0) = 0;

			protected:
				SessionPtr sessionPtr;
			};//class TcpSession
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_TCP_SESSION_H
