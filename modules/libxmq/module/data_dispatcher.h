//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : 数据分发模型
//
//		History:
//					1. 2022-01-21 由王科威创建
//					2. 2022-01-24 由王科威修改，在内部实现服务节点注册、节点查询和在线检测
//					3. 2022-04-04 由王科威修改，添加worker线程处理事务
//

#ifndef MODULE_NETWORK_XMQ_DATA_DISPATCHER_H
#define MODULE_NETWORK_XMQ_DATA_DISPATCHER_H

#include "xmq_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class DataDispatcher : public XMQNode
			{
			public:
				DataDispatcher(void);
				virtual ~DataDispatcher(void);

			public:
				int run(
					const XMQNodeConf& conf, 
					void* ctx = nullptr) override;
				int stop(void) override;
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* name = nullptr) override;

			protected:
				void pollDataThread(void) override;

			private:
				void* rso;
				void* dso;
			};//class DataDispatcher
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_DISPATCHER_H
