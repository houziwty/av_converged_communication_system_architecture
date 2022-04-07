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
#include "any/any.h"
using namespace framework::utils::data;

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//添加
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int add(
					const void* data = nullptr, 
					const uint64_t bytes = 0);

				//获取消息
				//@index [in] : 索引号
				//@Return : 消息
				inline const Any* msg(const int32_t index = 0) const
				{
					return -1 < index && index < messages.size() ? messages[index] : nullptr;
				}

				//接收
				//@s [in] : socket
				//@Return : 错误码
				int recv(xsocket s = nullptr);

				//发送
				//@s [in] : socket
				//@Return : 错误码
				int send(xsocket s = nullptr);

				//转发
				//@rs [in] : 接收socket
				//@ss [in] : 发送socket
				//@Return : 错误码
				int forward(xsocket rs = nullptr, xsocket ss = nullptr);

			private:
				std::vector<Any*> messages;
			};//class Msg
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_MSG_H
