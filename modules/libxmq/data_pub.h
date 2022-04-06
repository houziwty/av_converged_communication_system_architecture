//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ数据发布角色
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_DATA_PUB_H
#define MODULE_NETWORK_XMQ_DATA_PUB_H

#include "async_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataPub : public AsyncNode
			{
			public:
				//number [in] : 发布数据缓存个数
				DataPub(
					const XMQModeConf& conf, 
					const uint32_t number = 3);
				~DataPub(void);

			public:
				int run(xctx c = nullptr) override;
				int stop(void) override;
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* /*id*/ = nullptr) override;
					
			protected:
				void pollDataThread(void) override;

			private:
				const uint32_t hwm;
				xsocket pso;
			};//class DataPub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_PUB_H
