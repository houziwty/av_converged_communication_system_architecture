//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-11
//		Description : 负载均衡任务模型
//
//		History:
//					1. 2022-04-11 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_WORKER_TASK_H
#define MODULE_NETWORK_XMQ_WORKER_TASK_H

#include "xmq_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class WorkerTask : public XMQNode
			{
			public:
				WorkerTask(PolledDataCallback callback);
				virtual ~WorkerTask(void);

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
				void* dso;
				void* poller;
			};//class WorkerTask
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_WORKER_TASK_H
