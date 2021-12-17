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

#ifndef FRAMEWORK_NETWORK_XMQ_WORKER_MODEL_H
#define FRAMEWORK_NETWORK_XMQ_WORKER_MODEL_H

#include "boost/shared_ptr.hpp"
#include "libxmq/worker.h"
using namespace module::network::xmq;
using WorkerPtr = boost::shared_ptr<Worker>;

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class WorkerModel
			{
			public:
				WorkerModel(void);
				virtual ~WorkerModel(void);

			public:
				//启动
				//@name [in] : 服务名称
				//@ip [in] : IP地址
				//@port [in] : 端口号
				//@Return : 错误码
				virtual int start(
					const std::string name, 
					const std::string ip, 
					const unsigned short port = 0);

				//停止
				//@Return : 错误码
				virtual int stop();

				//发送
				//@data [in] : 数据
				//@Return : 错误码
				virtual int send(const std::string data);

			protected:
				//工作者模型数据接收回调函数
				//@data [out] : 数据
				virtual void afterWorkerPolledDataHandler(const std::string data) = 0;

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
				std::string serviceName;
			};//class WorkerModel
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_WORKER_MODEL_H
