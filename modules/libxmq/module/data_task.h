//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-24
//		Description : 数据任务模型
//
//		History:
//					1. 2022-01-24 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_DATA_TASK_H
#define MODULE_NETWORK_XMQ_DATA_TASK_H

#include "xmq_node.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			typedef boost::function<void(const bool)> CheckOnlineStatusCallback;
			typedef boost::function<void(const char**, const uint32_t)> ServiceCapabilitiesNotificationCallback;

			class DataTask : public XMQNode
			{
			public:
				DataTask(
					PolledDataCallback poll, 
					CheckOnlineStatusCallback check, 
					ServiceCapabilitiesNotificationCallback capabilities);
				virtual ~DataTask(void);

			public:
				int run(
					const XMQNodeConf& conf, 
					void* ctx = nullptr) override;
				int stop(void) override;
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* /*name */= nullptr) override;

			protected:
				void pollDataThread(void) override;
				void checkOnlineThread(const char* name = nullptr);

			private:
				//注册应答处理
				//@url [in] : 应答URL
//				void processRegisterResponseMessage(void* url = nullptr);

				//查询应答处理
				//@url [in] : 应答URL
//				void processQueryResponseMessage(void* url = nullptr);

				//转发业务处理
				//@data [in] : 数据
				//@bytes [in] : 大小
				// void processForwardCustomMessage(
				// 	const void* data = nullptr, 
				// 	const uint64_t bytes = 0);

			private:
				uint32_t id;
				uint64_t registerRepTickout;
				bool online;
				void* check;
				void* dso;
				CheckOnlineStatusCallback checkOnlineStatusCallback;
				ServiceCapabilitiesNotificationCallback serviceCapabilitiesNotificationCallback;
			};//class DataTask
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_DATA_TASK_H
