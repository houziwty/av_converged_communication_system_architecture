//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 路由模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_ROUTER_H
#define MODULE_NETWORK_XMQ_ROUTER_H

#include "sock.h"

namespace module
{
    namespace network
    {
		namespace xmq
		{
			class Router : public Sock
			{
			public:
				Router(void);
				virtual ~Router(void);

			public:
				//监听
				//@ctx [in] : XMQ上下文
				//@port [in] : 端口号
				//@hwm [in] : 数据缓存大小
				//@Return : socket实例
				void* bind(
					void* ctx = nullptr, 
					const uint16_t port = 0, 
					const uint64_t hwm = 0);
			};//class Router
		}//namespace xmq
    }//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_ROUTER_H
