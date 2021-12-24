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
				//设置地址
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 数据深拷贝
				int address(const void* data = nullptr, const int bytes = 0);

				//获取地址
				//@Return : 地址
				inline const void* address_data(void) const
				{
					return addressData;
				}

				//获取地址大小
				//@Return : 地址大小
				inline const int address_bytes(void) const
				{
					return addressBytes;
				}

				//设置内容
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 数据深拷贝
				int content(const void* data = nullptr, const int bytes = 0);

				//获取内容
				//@Return : 内容
				inline const void* content_data(void) const
				{
					return contentData;
				}

				//获取内容大小
				//@Return : 内容大小
				inline const int content_bytes(void) const
				{
					return contentBytes;
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
				void* addressData;
				int addressBytes;
				void* contentData;
				int contentBytes;
			};//class Msg
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_MSG_H
