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
			typedef boost::function<void(const int, const int)> AsyncSendEventCallback;

			//数据接收回调
			//@_1 : socket错误码
			//@_2 : 数据
			//@_3 : 接收字节数
			typedef boost::function<void(const int, const void*, const int)> AsyncReceiveEventCallback;

			class IoSession
			{
			public:
				IoSession(void);
				virtual ~IoSession(void);

			public:
				//创建
				//@bytes [in] : 缓存大小，默认1MB
				//@scb [in] : 数据发送回调
				//@Return : 数据接收回调
				virtual int createNew(
					const int bytes = 1048576, 
					AsyncSendEventCallback scb = nullptr,
					AsyncReceiveEventCallback rcb = nullptr);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);
				
				//发送
				//@s : socket
				//@data : 数据
				//@bytes : 大小
				//@Return : 错误码
				int send(
					boost::asio::ip::tcp::socket& s, 
					const void* data = nullptr,
					const int bytes = 0);

				//接收
				//@s : socket
				//@Return : 错误码
				int receive(boost::asio::ip::tcp::socket& s);

			private:
				void* buffer;
				int bufBytes;
				AsyncSendEventCallback asyncSendEventCallback;
				AsyncReceiveEventCallback asyncReceiveEventCallback;
			};//class IoSession
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_IO_SESSION_H
