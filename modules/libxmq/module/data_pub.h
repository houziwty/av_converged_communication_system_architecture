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

#ifndef MODULE_NETWORK_XMQ_DATA_PUB_H
#define MODULE_NETWORK_XMQ_DATA_PUB_H

#include "xmq_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataPub : public XMQNode
			{
			public:
				DataPub(void);
				virtual ~DataPub(void);

			public:
				int run(
					const XMQNodeConf& conf, 
					void* ctx = nullptr) override;
				int stop(void) override;
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* /*name*/ = nullptr) override;
					
			protected:
				void pollDataThread(void) override;

			private:
				void* pso;
			};//class DataPub
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_PUB_H
