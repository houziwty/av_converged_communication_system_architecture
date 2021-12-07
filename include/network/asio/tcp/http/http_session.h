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

#ifndef FRAMEWORK_NETWORK_ASIO_HTTP_SESSION_H
#define FRAMEWORK_NETWORK_ASIO_HTTP_SESSION_H

#include "network/asio/tcp/tcp_session.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class HttpSession 
				: public TcpSession
			{
			public:
				HttpSession(IoSessionPtr sess);
				virtual ~HttpSession(void);
			};//class HttpSession
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_HTTP_SESSION_H
