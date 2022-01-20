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

#ifndef MODULE_NETWORK_HTTP_HTTP_SESSION_H
#define MODULE_NETWORK_HTTP_HTTP_SESSION_H

#include "tcp/tcp_session.h"
using namespace module::network::asio;
#include "defs.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class NETWORK_HTTP_EXPORT HttpSession 
				: public TcpSession
			{
			public:
				HttpSession(const std::string id);
				virtual ~HttpSession(void);

			protected:
				void fetchSentDataEventNotification(
					const int e = 0, 
					const int bytes = 0) override;
				void fetchReceivedDataEventNotification(
					const int e = 0, 
					const void* data = nullptr, 
					const int bytes = 0) override;

			private:
				const std::string uuid;
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
