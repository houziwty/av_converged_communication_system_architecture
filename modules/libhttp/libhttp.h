//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : HTTP协议库
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_LIB_HTTP_H
#define MODULE_NETWORK_HTTP_LIB_HTTP_H

#include "libhttp_defs.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class NETWORK_HTTP_EXPORT Libhttp
			{
			public:
				Libhttp(void);
				virtual ~Libhttp(void);

			public:
				//添加会话
				//@id [in] : 会话ID, >0
				//@Return : 错误码
				int addSession(const uint32_t id = 0);

				//删除会话
				//@id [in] : 会话ID
				//@Return : 错误码
				int removeSession(const uint32_t id = 0);

				//请求
				//@id [in] : 会话ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int request(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);
			};//class Libhttp
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_LIB_HTTP_H
