//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步数据交换模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_SWITCHER_H
#define MODULE_NETWORK_XMQ_SWITCHER_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class Switcher
			{
			public:
				//@callback [in] : 数据接收回调
				Switcher(PolledDataWithIDCallback callback = nullptr);
				~Switcher(void);

			public:
				//监听
				//@port [in] : 端口号
				//@Return : 错误码
				int bind(const unsigned short port = 0);

				//发送
				//@uid [in] : 用户ID标识
				//@data [in] : 数据
				//@Return : 错误码
				int send(
					const std::string uid, 
					const std::string data);

				//拉取
				//@Comment : 该模型必须由调用者负责主动拉取数据
				int poll(void);

			private:
				ctx_t* ctx;
				socket_t* router;
				PolledDataWithIDCallback handler;
			};//class Switcher
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SWITCHER_H
