//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ服务发现角色
//
//		History:
//					1. 2022-01-21 由王科威创建
//					2. 2022-01-24 由王科威修改，在内部实现服务节点注册、节点查询和在线检测
//					3. 2022-04-04 由王科威修改，添加worker线程处理事务
//

#ifndef MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
#define MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H

#include "async_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class ServiceDiscover : public AsyncNode
			{
			public:
				ServiceDiscover(
					const XMQModeConf& conf, 
					PolledDataCallback callback);
				~ServiceDiscover(void);

			public:
				int run(xctx c = nullptr) override;
				int stop(void) override;

				//发送数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@id [in] : 接收端ID
				//@Return : 错误码
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* id = nullptr) override;

			protected:
				void pollDataThread(void) override;

			private:
				//创建任务处理线程
				int createTaskThread(xctx c = nullptr);

				//销毁任务处理线程
				int destroyTaskThread(void);

				//任务线程
				void processTaskThread(xctx c = nullptr);

			private:
				const char* taskName{"inproc://service_dispatcher_task"};
				const uint8_t taskNumber;
				xsocket rso;
				xsocket dso;
				xthread tasks[32];
			};//class ServiceDiscover
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
