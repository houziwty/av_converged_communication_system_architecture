//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : HTTP服务端
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_HTTP_SERVER_H
#define FRAMEWORK_NETWORK_ASIO_HTTP_SERVER_H

#include "network/asio/tcp/tcp_server.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class HttpServer 
				: public TcpServer
			{
			public:
				HttpServer(void);
				virtual ~HttpServer(void);

			protected:
				void fetchAcceptedEventNotification(IoSessionPtr sessionPtr, const int e = 0) override;
			};//class HttpServer
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_HTTP_SERVER_H
