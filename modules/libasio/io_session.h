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

#ifndef MODULE_NETWORK_ASIO_IO_SESSION_H
#define MODULE_NETWORK_ASIO_IO_SESSION_H

#include "boost/asio.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			//数据发送回调
			//@_1 : socket错误码
			//@_2 : 发送字节数
			typedef boost::function<void(const int, const int)> SentDataEventCallback;

			//数据接收回调
			//@_1 : socket错误码
			//@_2 : 数据
			//@_3 : 接收字节数
			typedef boost::function<void(const int, const void*, const int)> ReceivedDataEventCallback;

			class IoSession
			{
			public:
				//@ctx : 上下文
				IoSession(boost::asio::io_context& ctx);
				virtual ~IoSession(void);

			public:
				//创建
				//@bytes [in] : 缓存大小，默认1MB
				//@sent [in] : 数据发送回调
				//@recved [in] : 数据接收回调
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
			};//class IoSession
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_IO_SESSION_H
