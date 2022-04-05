//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ数据订阅角色
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_DATA_SUB_H
#define MODULE_NETWORK_XMQ_DATA_SUB_H

#include "async_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataSub : public AsyncNode
			{
			public:
				DataSub(
					const XMQModeConf& conf, 
					PolledDataCallback callback);
				~DataSub(void);

			public:
				int run(xctx c = nullptr) override;
				int stop(void) override;
				int send(
					const void* /*data = nullptr*/, 
					const uint64_t /*bytes = nullptr*/, 
					const char* /*id = 0*/) override;
					
			protected:
				void pollDataThread(void) override;

			private:
				xsocket sso;
			};//class DataSub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_SUB_H
