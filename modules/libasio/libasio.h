//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : 异步网络通信库
//
//		History:
//					1. 2022-01-21 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_LIB_ASIO_H
#define MODULE_NETWORK_ASIO_LIB_ASIO_H

#include "libasio_defs.h"

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

				//删除会话
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
				//@Comment : ip和port参数仅当UDP时有效
				virtual int send(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* ip = nullptr, 
					const uint16_t port = 0);
			
			protected:
				//监听事件通知
				//@remoteIP [out] : 远程IP
				//@remotePort [out] : 远程端口号
				//@localPort [out] : 本地端口号
				//@e [out] : 错误码
				//@Return : 会话ID，>0有效
				virtual uint32_t afterFetchIOAcceptedEventNotification(
					const char* remoteIP = nullptr, 
					const uint16_t remotePort = 0, 
					const uint16_t localPort = 0, 
					const int32_t e = 0) = 0;

				//连接事件通知
				//@e [out] : 错误码
				//@user [out] : 用户数据
				//@Return : 会话ID，>0有效
				virtual uint32_t afterFetchIOConnectedEventNotification(
					const int32_t e = 0, 
					void* user = nullptr) = 0;

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
			};//class Libasio
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_LIB_ASIO_H
