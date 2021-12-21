//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : TCP服务端
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_TCP_SERVER_H
#define MODULE_NETWORK_ASIO_TCP_SERVER_H

#include "service.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT TcpServer
			{
			public:
				TcpServer(void);
				virtual ~TcpServer(void);

			public:
				//启动
				//@port [in] : 端口号
				//@Return : 错误码
				virtual int start(const unsigned short port = 0);

				//停止
				//@Return : 错误码
				virtual int stop(void);

			protected:
				//监听事件通知
				//@sessionPtr [in] : 会话
				//@e [in] : socket错误码
				virtual void fetchAcceptedEventNotification(SessionPtr sessionPtr, const int e = 0) = 0;

			private:
				Service service;
			};//class TcpServer
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_TCP_SERVER_H
