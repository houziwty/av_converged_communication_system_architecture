//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ角色
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_XMQ_ROLE_H
#define MODULE_NETWORK_XMQ_XMQ_ROLE_H

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
			
			class XMQRole
			{
			public:
				XMQRole(
					const XMQModeConf& conf, 
					PolledDataCallback callback);
				virtual ~XMQRole(void);

			public:
				//运行
				//c [in] : XMQ上下文参数
				//@Return : 错误码
				virtual int run(ctx_t c = nullptr);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//发送数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@id [in] : 接收端ID
				//@Return : 错误码
				virtual int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* id = nullptr) = 0;

			protected:
				//读取数据线程
				virtual void pollDataThread(void) = 0;
				//检测服务在线状态线程
				virtual void checkServiceOnlineStatusThread(void) = 0;

			protected:
				const XMQModeConf modeconf;
				socket_t so;
				_thread_t poller;
				_thread_t check;
				bool stopped;
				PolledDataCallback polledDataCallback;
			};//class XMQRole
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_XMQ_ROLE_H
