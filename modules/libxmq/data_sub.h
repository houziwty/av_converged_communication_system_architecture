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

#include "xmq_role.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataSub : public XMQRole
			{
			public:
				DataSub(
					const XMQModeConf& conf, 
					PolledDataCallback pollcb);
				~DataSub(void);

			public:
				int run(ctx_t c = nullptr) override;
				int stop(void) override;
				int send(
					const char* /*name = nullptr*/, 
					const void* /*data = nullptr*/, 
					const uint64_t /*bytes = 0*/) override;
					
			protected:
				void pollDataThread(void) override;
				void checkServiceOnlineStatusThread(void) override;
			};//class DataSub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_SUB_H
