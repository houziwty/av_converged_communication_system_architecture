//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 异步会话模型
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_SESSION_H
#define MODULE_NETWORK_ASIO_SESSION_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class NETWORK_ASIO_EXPORT Session
			{
			public:
				//@ctx : 上下文
				Session(boost::asio::io_context& ctx);
				virtual ~Session(void);

			public:
				//创建
				//@sent [in] : 数据发送回调
				//@recved [in] : 数据接收回调
				//@bytes [in] : 缓存大小，默认1MB
				//@Return : 错误码
				virtual int createNew(
					SentDataEventCallback sent, 
					ReceivedDataEventCallback recved, 
					const unsigned int bytes = 1048576);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);
				
				//发送
				//@data : 数据
				//@bytes : 大小
				//@Return : 错误码
				int send(
					const void* data = nullptr,
					const int bytes = 0);

				//接收
				//@Return : 错误码
				int receive(void);

				//获取socket实例
				inline boost::asio::ip::tcp::socket& sock(void)
				{
					return so;
				}

			private:
				boost::asio::ip::tcp::socket so;
				void* buffer;
				unsigned int bufBytes;
				SentDataEventCallback sentDataEventCBFunc;
				ReceivedDataEventCallback receivedDataEventCBFunc;
			};//class Session
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_SESSION_H
