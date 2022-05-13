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

#include "libhttp_defs.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class HttpSession
			{
			public:
				HttpSession(void);
				virtual ~HttpSession(void);

			public:
				//处理HTTP请求
				//@data [in] : 请求消息
				//@bytes [in] : 大小
				//@Return : 错误码
				int request(
					const void* data = nullptr, 
					const uint64_t bytes = 0);

				//处理HTTP应答
				//@id [in] : 节点ID
				//@code [in] : HTTP状态码
				//@status [in] : HTTP状态描述
				//@headers [in] : 应答头字段
				//@number [in] : 应答头字段个数
				//@data [in] : 应答体
				//@bytes [in] : 应答体字节数
				//@Return : 错误码
				//@Comment : 数据通过该方法构建HTTP请求再由方法发送
				// int response(
				// 	const uint32_t id = 0, 
				// 	const int32_t code = 0, 
				// 	const char* status = nullptr, 
				// 	const HttpResponseHeader* headers = nullptr, 
				// 	const uint32_t number = 0, 
				// 	const void* data = nullptr, 
				// 	const uint64_t bytes = 0);

			private:
				// static int afterHttpReadDataNotification(
				// 	void* ctx = nullptr, 
				// 	http_session_t* session = nullptr, 
				// 	const char* method = nullptr, 
				// 	const char* path = nullptr);
			};//class HttpSession
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SESSION_H
