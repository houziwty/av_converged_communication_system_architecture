//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 线程类
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_UTILS_THREAD_THREAD_H
#define FRAMEWORK_UTILS_THREAD_THREAD_H

namespace framework
{
	namespace utils
	{
		namespace thread
		{
			typedef void thread_t;

			class Thread
			{
			public:
				Thread(void);
				~Thread(void);

			public:
				//设置当前线程CPU亲和度
				//@t [in] : 线程实例
				//@idx [in] : CPU索引号
				//@Comment : 如果idx >= 0表示设置, idx == -1表示取消设置
				int setAffinity(thread_t* t = nullptr, const int idx = 0);

				//等待结束
				//@t [in] : 线程实例
				void join(thread_t* t = nullptr);
			};//class Thread
		}//namespace thread
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_THREAD_THREAD_H
