//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ角色
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_XMQ_ROLE_H
#define MODULE_NETWORK_XMQ_XMQ_ROLE_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class XMQRole
			{
			public:
				XMQRole(void);
				virtual ~XMQRole(void);

			public:
				//运行
				//c [in] : XMQ上下文参数
				//@Return : 错误码
				virtual int run(ctx_t c = nullptr);

				//停止
				//@Return : 错误码
				virtual int stop(void);

			protected:
				//读取数据线程
				virtual void pollDataThread(void) = 0;

			protected:
				socket_t so;
				thread_t poller;
				bool stopped;
			};//class XMQRole
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_XMQ_ROLE_H
