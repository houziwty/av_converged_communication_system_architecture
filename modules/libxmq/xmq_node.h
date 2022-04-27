//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ节点
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_XMQ_NODE_H
#define MODULE_NETWORK_XMQ_XMQ_NODE_H

#include "boost/function.hpp"
#include "libxmq_defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			//接收数据通知
			//@_1 [out] : 节点ID
			//@_2 [out] : 数据 
			//@_3 [out] : 大小
			//@_4 [out] : 数据源名称
			typedef boost::function<void(const uint32_t, const void*, const uint64_t, const char*)> PolledDataCallback;
			
			class XMQNode
			{
			public:
				XMQNode(PolledDataCallback callback);
				virtual ~XMQNode(void);

			public:
				//运行
				//conf [in] : 配置
				//ctx [in] : XMQ上下文
				//@Return : 错误码
				virtual int run(
					const XMQNodeConf& conf, 
					void* ctx = nullptr);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//发送
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@name [in] : 接收端名称
				//@Return : 错误码
				virtual int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* name = nullptr) = 0;

			protected:
				//读取线程
				virtual void pollDataThread(void) = 0;

			protected:
				void* poller;
				bool stopped;
				PolledDataCallback polledDataCallback;
			};//class XMQNode
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_XMQ_NODE_H
