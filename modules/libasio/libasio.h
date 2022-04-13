//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : ASIO库
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_LIB_ASIO_H
#define MODULE_NETWORK_ASIO_LIB_ASIO_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT Libasio
			{
			public:
				Libasio(void);
				virtual ~Libasio(void);

			public:
				//添加配置
				//@conf [in] : 配置参数
				//@Return : 错误码
				int addConf(const ASIOModeConf& conf);

				//删除配置
				//@id [in] : 会话ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//发送数据
				//@id [in] : 会话ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@ip [in] : 远程IP
				//@port [in] : 远程端口号
				//@Return : 错误码
				//@Comment : ip和port参数仅当UDP会话时有效
				virtual int send(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* ip = nullptr, 
					const uint16_t port = 0);
			
			protected:
				//监听事件通知
				//@ip [out] : 远程IP
				//@port [out] : 远程端口号
				//@e [out] : 错误码
				//@Return : 会话ID，>0有效
				virtual uint32_t afterFetchIOAcceptedEventNotification(
					const char* ip = nullptr, 
					const uint16_t port = 0, 
					const int32_t e = 0) = 0;

				//连接事件通知
				//@e [out] : 错误码
				//@Return : 会话ID，>0有效
				virtual uint32_t afterFetchIOConnectedEventNotification(const int32_t e = 0) = 0;

				//接收数据通知
				//@id [out] : 会话ID
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@e [out] : 错误码
				virtual void afterPolledIOReadDataNotification(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const int32_t e = 0) = 0;

				//发送数据通知
				//@id [out] : 会话ID
				//@bytes [out] : 大小
				//@e [out] : 错误码
				virtual void afterPolledIOSendDataNotification(
					const uint32_t id = 0, 
					const uint64_t bytes = 0, 
					const int32_t e = 0) = 0;
			};//class ASIONode
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_LIB_ASIO_H
