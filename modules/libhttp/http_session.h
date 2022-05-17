//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-27
//		Description : HTTP会话
//
//		History:
//					1. 2022-03-27 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_HTTP_SESSION_H
#define MODULE_NETWORK_HTTP_HTTP_SESSION_H

#include "splitter/http_request_splitter.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class HttpSession : protected HttpRequestSplitter
			{
			public:
				HttpSession(void);
				virtual ~HttpSession(void);

			public:
				int input(
					const char* data = nullptr, 
                    const std::size_t len = std::string::npos) override;

			protected:
				const std::size_t afterRecvHttpHeaderNotification(
                    const char* data = nullptr, 
                    const std::size_t bytes = 0) override;
				void afterRecvHttpContentNotification(
                    const char* data = nullptr, 
                    const std::size_t bytes = 0) override;
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
