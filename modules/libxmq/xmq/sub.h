//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ数据订阅模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_SUB_H
#define MODULE_NETWORK_XMQ_SUB_H

#include <string>

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class Sub
			{
			public:
				Sub(void);
				~Sub(void);

			public:
				//连接
				//@ip [in] : 远程IP
				//@port [in] : 端口号
				//@c [in] : XMQ上下文实例
				//@Return : socket实例
				void* connect(
					const std::string ip,
					const unsigned short port = 0,
					void* c = nullptr);

				//关闭
				//@s [in] : socket实例
				//@Return : 错误码
				int shutdown(void* s = nullptr);
			};//class Sub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SUB_H
