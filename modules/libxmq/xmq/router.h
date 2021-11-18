//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ路由模型类
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_ROUTER_H
#define MODULE_NETWORK_XMQ_ROUTER_H

namespace module
{
    namespace network
    {
		namespace xmq
		{
			class Router
			{
			public:
				Router(void);
				~Router(void);

			public:
				//监听
				//@c [in] : XMQ上下文实例
				//@port [in] : 端口号
				//@Return : socket实例
				void* bind(
					void* c = nullptr, 
					const unsigned short port = 0);

				//关闭
				//@s [in] : socket实例
				//@Return : 错误码
				int shutdown(void* s = nullptr);
			};//class Router
		}//namespace xmq
    }//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_ROUTER_H
