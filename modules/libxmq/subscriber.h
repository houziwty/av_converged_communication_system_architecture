//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步订阅模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_SUBSCRIBER_H
#define MODULE_NETWORK_XMQ_SUBSCRIBER_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Subscriber
			{
			public:
				//@callback [in] : 数据接收回调
				Subscriber(PolledDataWithoutIDCallback callback);
				~Subscriber(void);

			public:
				//连接
				//@ip [in] : 远程IP
				//@port [in] : 端口号
				//@hwm [in] : 缓存大小
				//@Return : 错误码
				int connect(
					const char* ip = nullptr, 
					const uint16_t port = 0,
					const int32_t hwm = 10);

				//拉取
				//@Comment : 该模型必须由调用者负责主动拉取数据
				int poll(void);

			private:
				ctx_t ctx;
				socket_t sub;
				PolledDataWithoutIDCallback handler;
			};//class Subscriber
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SUBSCRIBER_H
