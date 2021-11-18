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
#include <string>

namespace module
{
	namespace network
	{
		namespace xmq
		{
			typedef void socket_t;

			class Msg
			{
			public:
				Msg(void);
				~Msg(void);

			public:
				//添加
				//@data [in] : 数据
				//@Comment : 无需线程安全
				void pushBack(const std::string data);

				//移除并获取数据
				//@Return : 数据
				//@Comment : 无需线程安全
				const std::string popFront(void);

				//接收
				//@s : 接收对象
				//@Return : 错误码
				int recv(void* s = nullptr);

				//发送
				//@s : 发送对象
				//@Return : 错误码
				int send(void* s = nullptr);

			private:
				std::vector<std::string> msgs;
			};//class Msg
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_MSG_H
