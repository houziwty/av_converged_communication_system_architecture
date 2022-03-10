//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 异步定时器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_UTILS_TIME_ASYNC_TIMER_H
#define FRAMEWORK_UTILS_TIME_ASYNC_TIMER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace utils
	{
		namespace time
		{
			class Service;

			//超时事件回调
			//@_1 : 错误码
			typedef boost::function<void(const int)> AsyncExpireEventCallback;

			class AsyncTimer 
				: public boost::enable_shared_from_this<AsyncTimer>
			{
			public:
				AsyncTimer(
					AsyncExpireEventCallback callback);
				~AsyncTimer(void);

			public:
				//运行
				//@ios [in] : IO服务
				//@callback [in] : 监听回调
				//@expire : 超时时间，单位：毫秒
				//@Return : 错误码
				int run(
					boost::asio::io_context* ctx = nullptr, 
					const int expire = 5000);

			private:
				AsyncExpireEventCallback asyncExpireEventCallback;
			};//class AsyncTimer
		}//namespace time
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_TIME_ASYNC_TIMER_H
