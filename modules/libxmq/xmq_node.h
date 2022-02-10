//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ节点
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_XMQ_NODE_H
#define MODULE_NETWORK_XMQ_XMQ_NODE_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT XMQNode
			{
			public:
				XMQNode(void);
				virtual ~XMQNode(void);

			public:
				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const XMQModeConf& conf);

				//删除角色
				//@id [in] : 角色ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//运行
				//@Return : 错误码
				virtual int run(void);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//发送数据
				//@id [in] : 角色ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@to [out] : 数据目的地
				//@Return : 错误码
				virtual int send(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* to = nullptr);
			
			protected:
				//接收数据通知
				//@id [out] : 角色ID
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@from [out] : 数据来源
				virtual void afterPolledDataNotification(
					const uint32_t id = 0, 
					const void* data = nullptr,  
					const uint64_t bytes = 0, 
					const char* from = nullptr) = 0;

				//在线状态通知
				//@online [out] : true表示在线，false表示离线
				virtual void afterFetchOnlineStatusNotification(const bool online = false) = 0;

				//服务信息通知
				//@infos [out] : 服务信息集合 
				//@number [out] : 服务个数
				virtual void afterFetchServiceCapabilitiesNotification(
					const ServiceInfo* infos = nullptr, 
					const uint32_t number = 0) = 0;
			};//class XMQNode
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_XMQ_NODE_H
