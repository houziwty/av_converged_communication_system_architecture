//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 异步工作者模型
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_XMQ_WORKER_MODE_H
#define FRAMEWORK_NETWORK_XMQ_WORKER_MODE_H

#include "boost/shared_ptr.hpp"
#include "worker.h"
using namespace module::network::xmq;
using WorkerPtr = boost::shared_ptr<Worker>;

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class WorkerMode
			{
			public:
				WorkerMode(void);
				virtual ~WorkerMode(void);

			public:
				//启动
				//@name [in] : 服务名称
				//@bytes [in] : 大小
				//@ip [in] : IP地址
				//@port [in] : 端口号
				//@Return : 错误码
				virtual int start(
					const void* name = nullptr, 
					const int bytes = 0,  
					const char* ip = nullptr, 
					const unsigned short port = 0);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//发送
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				virtual int send(const void* data = nullptr, const int bytes = 0);

			protected:
				//工作者模型数据接收回调函数
				//@data [out] : 数据
				//@bytes [out] : 大小
				virtual void afterWorkerPolledDataHandler(const void* data = nullptr, const int bytes = 0) = 0;

			private:
				//工作者模型数据读取线程
				void pollDataFromWorkerThread(void);

				//工作者模型心跳线程
				void keepAliveWorkerThread(void);

			protected:
				WorkerPtr workerPtr;
				void* pollDataThread;
				void* keepaliveThread;
				bool stopped;
				enum
				{
					Max = 256
				};
				char serviceName[Max];
			};//class WorkerMode
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_WORKER_MODE_H
