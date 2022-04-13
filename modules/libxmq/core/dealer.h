//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步工作者模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_DEALER_H
#define MODULE_NETWORK_XMQ_DEALER_H

#include "sock.h"

namespace module
{
    namespace network
    {
		namespace xmq
		{
			class Dealer : public Sock
			{
			public:
				Dealer(void);
				virtual ~Dealer(void);

			public:
				//连接
				//@ctx [in] : XMQ上下文
				//@uid [in] : 名字
				//@ip [in] : 远程IP
				//@port [in] : 端口号
				//@hwm [in] : 缓存大小
				//@reconivl [in] : 重连间隔
				//@Return : socket实例
				void* connect(
					void* ctx = nullptr, 
					const char* name = nullptr, 
					const char* ip = nullptr,
					const uint16_t port = 0, 
					const uint64_t hwm = 0, 
					const uint64_t reconivl = 0);

				//监听
				//@ctx [in] : XMQ上下文
				//@name [in] : 线程间通信名称
				//@hwm [in] : 缓存大小
				//@Return : socket实例
				void* bind(
					void* ctx = nullptr, 
					const char* name = nullptr, 
					const uint64_t hwm = 0);

				//连接
				//@ctx [in] : XMQ上下文
				//@name [in] : 线程间通信名称
				//@hwm [in] : 缓存大小
				//@Return : socket实例
				void* connect_inproc(
					void* ctx = nullptr, 
					const char* name = nullptr, 
					const uint64_t hwm = 0);
			};//class Dealer
		}//namespace xmq
    }//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DEALER_H
