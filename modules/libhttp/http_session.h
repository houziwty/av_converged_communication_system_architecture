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

#include "boost/function.hpp"
#include "parser/http_request_parser.h"
#include "wrapper/http_response_wrapper.h"
#include "memory/xbuf.h"
using namespace framework::utils::memory;

namespace module
{
	namespace network
	{
		namespace http
		{
			//HTTP请求回调
			//@_1 [out] : 会话ID
			//@_2 [out] : URL
			//@_3 [out] : Content类型
			//@_4 [out] : Content
			typedef boost::function<void(const uint32_t, const char*, const char*, const char*)> AfterFetchHttpRequestCallback;

			class HttpSession
			{
			public:
				HttpSession(
					const uint32_t id = 0, 
					AfterFetchHttpRequestCallback callback = nullptr);
				virtual ~HttpSession(void);

			public:
				//输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int input(
					const void* data = nullptr, 
                    const uint32_t bytes = 0);

			private:
				void afterParsedHttpRequestHandler(
					const std::string& method, 
					const std::string& url, 
					const std::string& protocol, 
					const std::multimap<std::string, std::string>& headers, 
					const std::string& content);
				
				//发送HTTP应答
				//@code [in] : HTTP错误码
				//@headers [in] : 消息头字段集合
				//@content [in] : 消息体
				// void sendHttpResponse(
				// 	const int code, 
				// 	std::unordered_map<std::string, std::string>& headers,
				// 	const std::string content) const;

				//构建OPTIONS应答
				void makeHttpCommandOptionsResponse(
					std::multimap<std::string, std::string>& headers) const;
				//处理POST命令
				// void afterFetchHttpRequestPost(
				// 	const std::string& url, 
				// 	const std::string& protocol, 
				// 	const std::string& content, 
				// 	std::unordered_map<std::string, std::string>& headers);

				//处理GET命令
				void processHttpRequestGet(
					const std::string& url, 
					const std::string& protocol, 
					const std::multimap<std::string, std::string>& headers, 
					const std::string& content);

				//尝试升级会话为Websocket
				//@Return : true/false
				void tryUpgradeWebsocket(
					const std::multimap<std::string, std::string>& headers);
				//尝试获取跨域值
				//@Return : true/false
				void tryGetOriginValue(
					const std::multimap<std::string, std::string>& headers);

			private:
				XBuffer buffer;
				HttpRequestParser httpRequestParser;
				HttpResponseWrapper wrapper;
				const uint32_t sid;
				AfterFetchHttpRequestCallback afterFetchHttpRequestCallback;
				//是否Websocket
				std::string ws_sha1;
				std::string ws_proto;
				std::string ws_version;
				//是否跨域
				std::string origin;
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
