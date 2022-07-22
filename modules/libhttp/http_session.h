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
			//HTTP服务应答回调
			//@_1 [out] : 会话ID
			//@_2 [out] : 数据
			//@_3 [out] : 会话关闭标识
			typedef boost::function<void(const uint32_t, const char*, const bool)> AfterFetchHttpResponseCallback;

			//HTTP服务请求回调
			//@_1 [out] : 会话ID
			//@_2 [out] : URL
			//@_3 [out] : 请求参数类型
			//@_4 [out] : 请求参数数据
			//@_5 [in,out] : HTTP错误码
			//@_6 [in,out] : 应答参数类型
			//@_7 [in,out] : 应答参数数据
			typedef boost::function<void(const uint32_t, const char*, const char*, const char*, int&, char*&, char*&)> AfterFetchHttpRequestCallback;

			class HttpSession
			{
			public:
				HttpSession(
					const uint32_t id = 0, 
					AfterFetchHttpResponseCallback rep = nullptr, 
					AfterFetchHttpRequestCallback req = nullptr);
				virtual ~HttpSession(void);

			public:
				//输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int input(
					const void* data = nullptr, 
                    const uint32_t bytes = 0);

				//处理HTTP请求行和请求头
				//@method [out] : 方法
				//@url [out] : URL
				//@protocol [out] : 协议/版本
				void afterParsedHttpRequestHeaderHandler(
					const std::string& method, 
					const std::string& url, 
					const std::string& protocol, 
					const std::multimap<std::string, std::string>& headers);

				//处理HTTP请求消息体
				//@url [out] : URL
				//@protocol [out] : 协议/版本
				//@type [out] : 消息体类型
				//@content [out] : 消息体
				void afterParsedHttpRequestContentHandler(
					const std::string& url, 
					const std::string& protocol, 
					const std::string& type, 
					const std::string& content);

			private:
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
				void doHttpCommandGetProcess(
					const char* url, 
					const std::multimap<std::string, std::string>& headers, 
					int& e, 
					std::string& content, 
					std::string& rep);

				//尝试升级会话为Websocket
				//@Return : 升级标识
				bool tryUpgradeWebsocket(
					const std::multimap<std::string, std::string>& headers);

			private:
				XBuffer buffer;
				HttpRequestParser parser;
				HttpResponseWrapper wrapper;
				const uint32_t sid;
				AfterFetchHttpResponseCallback afterFetchHttpResponseCallback;
				AfterFetchHttpRequestCallback afterFetchHttpRequestCallback;
				//是否升级为Websocket会话
				bool websocket;
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
