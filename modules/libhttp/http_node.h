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

#ifndef MODULE_NETWORK_HTTP_HTTP_NODE_H
#define MODULE_NETWORK_HTTP_HTTP_NODE_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class NETWORK_HTTP_EXPORT HttpNode
			{
			public:
				HttpNode(void);
				virtual ~HttpNode(void);

			public:
				//添加节点
				//@id [in] : 节点ID
				//@Return : 错误码
				int add(const uint32_t id = 0);

				//删除节点
				//@id [in] : 节点ID
				//@Return : 错误码
				int remove(const uint32_t id = 0);

				//处理HTTP请求
				//@id [in] : 配置ID
				//@error [in] : socket错误码
				//@data [in] : 请求消息
				//@bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 由afterPolledReadDataNotification接收的数据通过该方法分析HTTP请求
				int request(
					const uint32_t id = 0, 
					const int32_t error_t = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);

				//处理HTTP应答
				//@id [in] : 配置ID
				//@code [in] : HTTP状态码
				//@status [in] : HTTP状态描述
				//@headers [in] : 应答头字段
				//@number [in] : 应答头字段个数
				//@data [in] : 应答体
				//@bytes [in] : 应答体字节数
				//@Return : 错误码
				//@Comment : 数据通过该方法构建HTTP请求再由方法发送
				int response(
					const uint32_t id = 0, 
					const int32_t code = 0, 
					const char* status = nullptr, 
					const HttpResponseHeader* headers = nullptr, 
					const uint32_t number = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);

				//接收数据通知
				//@id [out] : 会话ID
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@e [out] : 错误码
				virtual void afterPolledReadDataNotification(
					const uint32_t id = 0, 
					const char* method = nullptr, 
					const char* path = nullptr) = 0;
			};//class HttpNode
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_NODE_H
