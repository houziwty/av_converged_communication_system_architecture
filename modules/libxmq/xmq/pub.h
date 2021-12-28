//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ数据发布模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_PUB_H
#define MODULE_NETWORK_XMQ_PUB_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Pub
			{
			public:
				Pub(void);
				~Pub(void);

			public:
				//监听
				//@c [in] : XMQ上下文实例
				//@port [in] : 端口号
				//@hwm [in] : 缓存大小
				//@Return : socket实例
				socket_t bind(
					ctx_t c = nullptr, 
					const unsigned short port = 0,
					const int hwm = 10);

				//关闭
				//@s [in] : socket实例
				//@Return : 错误码
				int shutdown(socket_t s = nullptr);
			};//class Pub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_PUB_H
