//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : Socket
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_SOCK_H
#define MODULE_NETWORK_XMQ_SOCK_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class Sock
			{
			public:
				Sock(void);
				virtual ~Sock(void);

			public:
				//关闭
				//@s [in] : socket实例
				//@Return : 错误码
				int shutdown(void* s = nullptr);

				//接收
				//@s [in] : socket
				//@buffer [in] : 缓存
				//@bytes [in] : 大小
				//@more [in,out] : 结束标识
				//@Return : 接收大小
				int recv(
					void* s, 
					void* buffer, 
					const uint64_t bytes, 
					bool& more);

				//发送
				//@s [in] : socket
				//@buffer [in] : 缓存
				//@bytes [in] : 大小
				//@more [in] : 结束标识
				//@Return : 发送大小
				int send(
					void* s = nullptr, 
					const void* buffer = nullptr, 
					const uint64_t bytes = 0, 
					bool more = false);

				//转发
				//@rs [in] : 接收socket
				//@ss [in] : 发送socket
				//@Return : 错误码
				int forward(void* rs = nullptr, void* ss = nullptr);
			};//class Sock
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SOCK_H
