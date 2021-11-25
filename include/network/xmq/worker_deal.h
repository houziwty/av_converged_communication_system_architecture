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

#ifndef FRAMEWORK_NETWORK_XMQ_WORKER_DEAL_H
#define FRAMEWORK_NETWORK_XMQ_WORKER_DEAL_H

#include <string>

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class WorkerDeal
			{
			public:
				WorkerDeal(void);
				virtual ~WorkerDeal(void);

			public:
				//启动
				//@uid [in] : 用户ID标识
				//@ip [in] : 远程IP
				//@port [in] : 端口号
				//@Return : 错误码
				virtual int start(
					const std::string uid,
					const std::string ip, 
					const unsigned short port = 0);

				//停止
				//@Return : 错误码
				virtual int stop();

				//发送
				//@data [in] : 数据
				//@Return : 错误码
				virtual int send(const std::string data);

				//心跳
				//@name [in] : 名称
				//@interval [in] : 时间间隔，单位：秒
				//@Return : 错误码
				virtual int keepalive(const std::string name, const int interval = 30);

			protected:
				//工作者模型数据接收回调函数
				//@data [out] : 数据
				virtual void afterWorkerPolledDataHandler(const std::string data) = 0;

			private:
				//工作者模型数据读取线程
				void pollDataFromWorkerThread(void);
				//工作者模型心跳线程
				//@name [in] : 名称
				//@interval [in] : 时间间隔，单位：秒
				void keepAliveWorkerThread(const std::string name, const int interval = 30);

			private:
				void* worker;
				void* thread;
				bool stopped;
			};//class WorkerDeal
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_WORKER_DEAL_H
