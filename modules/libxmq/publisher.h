//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 数据发布模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_PUBLISHER_H
#define MODULE_NETWORK_XMQ_PUBLISHER_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Publisher
			{
			public:
				Publisher(void);
				~Publisher(void);

			public:
				//监听
				//@port [in] : 端口号
				//@hwm [in] : 缓存大小
				//@Return : 错误码
				int bind(
					const uint16_t port = 0,
					const int32_t hwm = 10);

				//发送
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int send(const void* data = nullptr, const uint64_t bytes = 0);

			private:
				ctx_t ctx;
				socket_t pub;
			};//class Publisher
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_PUBLISHER_H
