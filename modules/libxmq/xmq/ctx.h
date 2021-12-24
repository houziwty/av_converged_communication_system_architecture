//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ上下文
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_CTX_H
#define MODULE_NETWORK_XMQ_CTX_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Ctx
			{
			public:
				Ctx(void);
				~Ctx(void);

			public:
				//创建
				//@Return : 上下文实例
				ctx_t createNew(void);

				//销毁
				//@c : 上下文实例
				int destroy(ctx_t c = nullptr);
			};//class Ctx
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_CTX_H
