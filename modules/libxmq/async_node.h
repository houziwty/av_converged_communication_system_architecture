//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ异步节点
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_ASYNC_NODE_H
#define MODULE_NETWORK_XMQ_ASYNC_NODE_H

#include "boost/function.hpp"
#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			//接收数据通知
			//@_1 [out] : 角色ID
			//@_2 [out] : 数据 
			//@_3 [out] : 大小
			//@_4 [out] : 数据来源
			typedef boost::function<void(const uint32_t, const void*, const uint64_t, const char*)> PolledDataCallback;
			
			class AsyncNode
			{
			public:
				AsyncNode(
					const XMQModeConf& conf, 
					PolledDataCallback callback);
				virtual ~AsyncNode(void);

			public:
				//运行
				//c [in] : XMQ上下文参数
				//@Return : 错误码
				virtual int run(xctx c = nullptr);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				virtual int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* id = nullptr) = 0;

			protected:
				//读取线程
				virtual void pollDataThread(void) = 0;

			protected:
				const XMQModeConf& modeconf;
				xthread poller_t;
				bool stopped;
				PolledDataCallback polledDataCallback;
			};//class AsyncNode
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_ASYNC_NODE_H
