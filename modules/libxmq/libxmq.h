//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ库
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_LIB_XMQ_H
#define MODULE_NETWORK_XMQ_LIB_XMQ_H

#include "libxmq_defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Libxmq
			{
			public:
				Libxmq(void);
				virtual ~Libxmq(void);

			public:
				//添加节点
				//@conf [in] :	节点配置参数
				//				该参数由调用者负责管理
				//@Return : 错误码
				int addNode(const XMQNodeConf& conf);

				//删除节点
				//@id [in] : 节点ID
				//@Return : 错误码
				int removeNode(const uint32_t id = 0);

				//发送数据
				//@id [in] : 节点ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@name [in] : 接收端名称
				//@Return : 错误码
				virtual int send(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* name = nullptr);
			
			protected:
				//接收数据通知
				//@id [out] : 节点ID
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@name [out] : 发送端名称
				virtual void afterPolledXMQDataNotification(
					const uint32_t id = 0, 
					const void* data = nullptr,  
					const uint64_t bytes = 0, 
					const char* name = nullptr);

				//在线状态通知
				//@online [out] : true表示在线，false表示离线
				//@Comment : 状态变更时触发
				virtual void afterFetchOnlineStatusNotification(const bool online = false);

				//在线服务名称通知
				//@names [out] : 名称
				//@Comment : 间隔30秒触发
				virtual void afterFetchServiceCapabilitiesNotification(const char* names = nullptr);
			};//class Libxmq
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_LIB_XMQ_H
