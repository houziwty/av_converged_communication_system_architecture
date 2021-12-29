//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步订阅模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_XMQ_SUB_MODE_H
#define FRAMEWORK_NETWORK_XMQ_SUB_MODE_H

#include "boost/shared_ptr.hpp"
#include "subscriber.h"
using namespace module::network::xmq;
using SubscriberPtr = boost::shared_ptr<Subscriber>;

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class SubMode
			{
			public:
				SubMode(void);
				virtual ~SubMode(void);

			public:
				//启动
				//@ip [in] : 发布端IP地址
				//@port [in] : 端口号
				//@Return : 错误码
				virtual int start(
					const char* ip = nullptr, 
					const unsigned short port = 0);

				//停止
				//@Return : 错误码
				virtual int stop(void);

			protected:
				//工作者模型数据接收回调函数
				//@data [out] : 数据
				//@bytes [out] : 大小
				virtual void afterWorkerPolledDataHandler(const void* data = nullptr, const int bytes = 0) = 0;

			private:
				//工作者模型数据读取线程
				void pollDataFromWorkerThread(void);

			private:
				SubscriberPtr subscriberPtr;
				void* thread;
				bool stopped;
			};//class SubModel
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_SUB_MODE_H
