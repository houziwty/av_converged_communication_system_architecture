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

#include <vector>
#include "defs.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			typedef struct tagMessage_t
			{
				void* data;
				uint64_t bytes;
			}Message;

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
				//@Comment : 1.数据深拷贝
				//           2.可以添加空字节
				int append(
					const void* data = nullptr, 
					const uint64_t bytes = 0);

				//获取消息
				//@Return : 消息
				inline const Message* msg(const int32_t index = 0) const
				{
					return -1 < index && index < messages.size() ? &messages[index] : nullptr;
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
				std::vector<Message> messages;
			};//class Msg
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_MSG_H
