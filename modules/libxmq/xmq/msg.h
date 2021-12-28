//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : XMQ消息类
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_MSG_H
#define MODULE_NETWORK_XMQ_MSG_H

#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class NETWORK_XMQ_EXPORT Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//添加
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 数据深拷贝
				int append(const void* data = nullptr, const int bytes = 0);

				//获取消息段数据
				//@Return : 消息段数据
				inline const void* msg(const unsigned char index = 0) const
				{
					return index < counter ? (void*)msgs[2 * index + 1] : nullptr;
				}

				//获取消息段大小
				//@Return : 消息段大小
				inline const unsigned char msg_bytes(const unsigned char index = 0) const
				{
					return index < counter ? msgs[2 * index] : 0;
				}

				//清除
				void clear(void);

				//接收
				//@s : socket
				//@Return : 错误码
				int recv(socket_t s = nullptr);

				//发送
				//@s : socket
				//@Return : 错误码
				int send(socket_t s = nullptr);

			private:
				enum
				{
					Max = 128
				};
				int msgs[Max];
				unsigned char counter;
			};//class Msg
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_MSG_H
