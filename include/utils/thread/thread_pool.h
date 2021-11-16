//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					线程池类
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef UTILS_THREAD_THREAD_POOL_H
#define UTILS_THREAD_THREAD_POOL_H

#include "boost/function.hpp"
#include "boost/serialization/singleton.hpp"
#include "boost/thread.hpp"

namespace utils
{
	namespace thread
	{
		typedef void thread_t;
		//线程体
		typedef boost::function<void(void)> Function;

		class ThreadPool :
			public boost::serialization::singleton<ThreadPool>
		{
		public:
			ThreadPool(void);
			~ThreadPool(void);

		public:
			//创建线程
			//@f [in] : 线程体
			//@Return : 线程实例
			thread_t* createNew(Function f);

			//销毁线程
			//@t [in] : 线程实例
			int destroy(thread_t* t = nullptr);

		private:
			boost::thread_group pool;
		};//class ThreadPool
	}//namespace thread
}//namespace utils

#endif//UTILS_THREAD_THREAD_POOL_H
