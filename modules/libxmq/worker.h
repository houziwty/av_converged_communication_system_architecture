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

#ifndef MODULE_NETWORK_XMQ_WORKER_H
#define MODULE_NETWORK_XMQ_WORKER_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Worker
			{
			public:
				//@callback [in] : 数据接收回调
				Worker(PolledDataWithoutIDCallback callback);
				~Worker(void);

			public:
				//连接
				//@uid [in] : 用户ID标识
				//@ip [in] : 远程IP
				//@port [in] : 端口号
				//@Return : 错误码
				int connect(
					const std::string uid,
					const std::string ip, 
					const unsigned short port = 0);

				//发送
				//@data [in] : 数据
				//@Return : 错误码
				int send(const std::string data);

				//拉取
				//@Comment : 该模型必须由调用者负责主动拉取数据
				int poll(void);

			private:
				ctx_t* ctx;
				socket_t* dealer;
				PolledDataWithoutIDCallback handler;
			};//class Worker
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_WORKER_H
