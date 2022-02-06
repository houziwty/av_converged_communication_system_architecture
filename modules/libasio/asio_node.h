//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : ASIO节点
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_ASIO_NODE_H
#define MODULE_NETWORK_ASIO_ASIO_NODE_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT ASIONode
			{
			public:
				ASIONode(void);
				virtual ~ASIONode(void);

			public:
				//运行
				//@Return : 错误码
				//@Comment : 节点必须先运行才能添加或删除配置
				virtual int run(void);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const ASIOModeConf& conf);

				//删除角色
				//@id [in] : 会话ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//发送数据
				//@id [in] : 会话ID
				//@data [in] : 数据
				//@bytes [in] : 大小 
				//@Return : 错误码
				virtual int send(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);
			
			protected:
				//监听事件通知
				//@ip [out] : 远程IP
				//@port [out] : 远程端口号
				//@e [out] : 错误码
				//@Return : 会话ID
				virtual int32_t afterFetchAcceptedEventNotification(
					const char* ip = nullptr, 
					const uint16_t port = 0, 
					const int32_t e = 0) = 0;

				//接收数据通知
				//@id [out] : 会话ID
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@e [out] : 错误码
				virtual void afterPolledReadDataNotification(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const int32_t e = 0) = 0;

				//发送数据通知
				//@id [out] : 会话ID
				//@bytes [out] : 大小
				//@e [out] : 错误码
				virtual void afterPolledSendDataNotification(
					const uint32_t id = 0, 
					const uint32_t bytes = 0, 
					const int32_t e = 0) = 0;
			};//class ASIONode
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_ASIO_NODE_H
