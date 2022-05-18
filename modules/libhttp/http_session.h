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

#include <map>
#include <string>
#include "boost/function.hpp"
#include "splitter/http_request_splitter.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			//HTTP服务应答回调
			//@_1 [out] : 会话ID
			//@_2 [out] : 数据
			//@_3 [out] : 大小
			//@_4 [out] : 会话关闭标识
			typedef boost::function<void(const uint32_t, const void*, const uint64_t, const bool)> AfterFetchHttpResponseCallback;

			class HttpSession : protected HttpRequestSplitter
			{
			public:
				HttpSession(
					const uint32_t id = 0, 
					AfterFetchHttpResponseCallback callback = nullptr);
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

			private:
				//HTTP应答
				//@code [in] : HTTP错误码
				//@header [in] : 消息头数据
				//@body [in] : 消息体数据
				void response(
					const int code = 500, 
					const std::multimap<std::string, std::string>& header = std::multimap<std::string, std::string>(), 
					const char* body = nullptr);

				//处理OPTIONS命令
				void fetchHttpRequestOptions(void);

			private:
				const uint32_t sid;
				AfterFetchHttpResponseCallback afterFetchHttpResponseCallback;
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
