//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : Boost.Asio的两种多线程模型
//							 Multi-Service ：每一个线程持有一个io_service，每个线程调用各自io的run方法；
//													- 在多核的机器上，这种方案可以充分利用多个 CPU 核心；
//													- 某个 socket 描述符并不会在多个线程之间共享，所以不需要引入同步机制；
//													- 在 event handler 中不能执行阻塞的操作，否则将会阻塞掉io_service所在的线程。
//							 Single-Service ：全局持有一个io_service，让其在多个线程间共享；
//													- 在 event handler 中允许执行阻塞的操作 (例如数据库查询操作)。
//													- 线程数可以大于 CPU 核心数，譬如说，如果需要在 event handler 中执行阻塞的操作，
//													  为了提高程序的响应速度，这时就需要提高线程的数目。
//													- 由于多个线程同时运行事件循环(event loop)，所以会导致一个问题：即一个 socket 
//													  描述符可能会在多个线程之间共享，容易出现竞态条件 (race condition)。譬如说，如果
//													  某个 socket 的可读事件很快发生了两次，那么就会出现两个线程同时读同一个 socket 
//													  的问题 (可以使用strand解决这个问题)。
//
//		History:
//					1. 2021-11-15 由王科威创建，实现Multi-Service模型
//

#ifndef MODULE_NETWORK_ASIO_IO_SERVICE_H
#define MODULE_NETWORK_ASIO_IO_SERVICE_H

#include <memory>
#include <thread>
#include <vector>
#include "boost/asio.hpp"
#include "boost/noncopyable.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class IoService 
				: protected boost::noncopyable
			{
			public:
				IoService(void);
				virtual ~IoService(void);

			public:
				//创建
				//@Return : 错误码
				virtual int createNew(void);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);

				//获取线程数
				//@Return : 线程数
				inline const int getCount(void) const
				{
					return static_cast<int>(threads.size());
				}
				
				//获取空闲IO
				//@Return : 空闲IO
				boost::asio::io_context& getIdle(void);

			private:
				std::vector<boost::asio::io_context> ctxs;
				std::vector<std::unique_ptr<boost::asio::io_context::work>> works;
				std::vector<std::thread> threads;
				int idle;
			};//class IoService
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_IO_SERVICE_H
