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
				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const HTTPModeConf& conf);

				//删除角色
				//@id [in] : 会话ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 会话ID
				//@data [in] : 数据
				//@error [in] : socket错误码
				//@bytes [in] : 大小
				//@Return : 错误码
				virtual int input(
					const uint32_t id = 0, 
					const int32_t error = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);
			};//class HttpNode
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_NODE_H
