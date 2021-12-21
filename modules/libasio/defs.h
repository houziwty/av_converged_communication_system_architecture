//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : ASIO库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_DEFS_H
#define MODULE_NETWORK_ASIO_DEFS_H

#if defined(_WINDOWS)
#ifdef USE_MODULE_NETWORK_ASIO
#define NETWORK_ASIO_EXPORT __declspec(dllimport)
#else
#define NETWORK_ASIO_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_ASIO
#elif defined(__linux__)
#define NETWORK_ASIO_EXPORT
#endif//_WINDOWS

#include <vector>
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/thread.hpp"

namespace module
{
	namespace network
	{
        namespace asio
        {
            class Session;
            using SessionPtr = boost::shared_ptr<Session>;

            //监听/连接事件回调
			//@_1 : 会话
			//@_2 : 错误码
			typedef boost::function<void(SessionPtr, const int)> RemoteConnectedEventCallback;

            //数据发送回调
			//@_1 : 错误码
			//@_2 : 发送字节数
			typedef boost::function<void(const int, const int)> SentDataEventCallback;

			//数据接收回调
			//@_1 : 错误码
			//@_2 : 数据
			//@_3 : 接收字节数
			typedef boost::function<void(const int, const void*, const int)> ReceivedDataEventCallback;
        }//namespace asio
    }//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_DEFS_H