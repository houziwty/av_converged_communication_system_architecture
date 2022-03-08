//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 会话
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_SESSION_H
#define MODULE_NETWORK_ASIO_SESSION_H

#include "boost/system/error_code.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			//数据发送回调
			//@_1 : 会话ID
			//@_2 : 发送字节数
			//@_3 : 错误码
			typedef boost::function<void(const uint32_t, const uint64_t, const int32_t)> SentDataEventCallback;

			//数据接收回调
			//@_1 : 会话ID
			//@_2 : 数据
			//@_3 : 接收字节数
			//@_4 : 错误码
			typedef boost::function<void(const uint32_t, const void*, const uint64_t, const int32_t)> ReceivedDataEventCallback;

			class Session
			{
			public:
				//@s : socket
				//@id : 会话ID
				Session(
					void* s = nullptr, 
					const uint32_t id = 0);
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
					const uint64_t bytes = 1048576);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);
				
				//发送
				//@data : 数据
				//@bytes : 大小
				//@ip [in] : 远程IP
				//@port [in] : 远程端口号
				//@Return : 错误码
				virtual int send(
					const void* data = nullptr,
					const uint64_t bytes = 0, 
					const char* ip = nullptr, 
					const uint16_t port = 0) = 0;

				//接收
				//@Return : 错误码
				virtual int receive(void) = 0;

			protected:
				//异步IO数据读写回调
				//@e : socket错误码
				//@bytes : 大小
				//@Comment : 两个回调声明一样，为了清晰表达所以还是分开写
				void afterAsyncWriteSomeCallback(
					const boost::system::error_code e, 
					const std::size_t bytes_transferred);
				void afterAsyncReadSomeCallback(
					const boost::system::error_code e,
					const std::size_t bytes_transferred);

			protected:
				void* so;
				const uint32_t sid;
				void* buffer;
				uint64_t bufBytes;
				SentDataEventCallback sentDataEventCallback;
				ReceivedDataEventCallback receivedDataEventCallback;
			};//class Session
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_SESSION_H
