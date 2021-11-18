//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 线程池类
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_UTILS_THREAD_THREAD_POOL_H
#define FRAMEWORK_UTILS_THREAD_THREAD_POOL_H

#include "boost/function.hpp"
#include "boost/serialization/singleton.hpp"
#include "boost/thread.hpp"

namespace framework
{
	namespace utils
	{
		namespace thread
		{
			typedef void thread_t;
			//线程体
			typedef boost::function<void(void)> Function;

			class ThreadPool 
				: public boost::serialization::singleton<ThreadPool>
			{
			public:
				ThreadPool(void);
				~ThreadPool(void);

			public:
				//创建
				//@f [in] : 线程
				//@Return : 线程实例
				thread_t* createNew(Function f);

				//销毁
				//@t [in] : 线程实例
				int destroy(thread_t* t = nullptr);

			private:
				boost::thread_group pool;
			};//class ThreadPool
		}//namespace thread
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_THREAD_THREAD_POOL_H
