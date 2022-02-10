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

#include "xmq_role.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataPub : public XMQRole
			{
			public:
				DataPub(
					const XMQModeConf& conf, 
					const uint32_t hwm = 3);
				~DataPub(void);

			public:
				int run(ctx_t c = nullptr) override;
				int stop(void) override;
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* /*id*/ = nullptr) override;
					
			protected:
				void pollDataThread(void) override;
				void checkServiceOnlineStatusThread(void) override;

			private:
				const uint32_t dataHWM;
			};//class DataPub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_PUB_H
