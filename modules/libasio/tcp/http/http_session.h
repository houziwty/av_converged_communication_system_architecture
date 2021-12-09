//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : HTTP会话
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_HTTP_SESSION_H
#define MODULE_NETWORK_ASIO_HTTP_SESSION_H

#include "libasio/tcp/tcp_session.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT HttpSession 
				: public TcpSession
			{
			public:
				HttpSession(SessionPtr ptr);
				virtual ~HttpSession(void);
			};//class HttpSession
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_HTTP_SESSION_H
