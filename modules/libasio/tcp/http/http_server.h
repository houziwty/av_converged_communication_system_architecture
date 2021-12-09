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

#ifndef MODULE_NETWORK_ASIO_HTTP_SERVER_H
#define MODULE_NETWORK_ASIO_HTTP_SERVER_H

#include "libasio/tcp/tcp_server.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT HttpServer 
				: public TcpServer
			{
			public:
				HttpServer(void);
				virtual ~HttpServer(void);

			protected:
				void fetchAcceptedEventNotification(SessionPtr sessionPtr, const int e = 0) override;
			};//class HttpServer
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_HTTP_SERVER_H
