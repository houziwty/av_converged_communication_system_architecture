//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					线程类
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef UTILS_THREAD_THREAD_H
#define UTILS_THREAD_THREAD_H

namespace utils
{
	namespace thread
	{
		class Thread
		{
		public:
			Thread(void);
			~Thread(void);

		public:
			//设置当前线程CPU亲和度
			//@idx [in] : CPU索引号
			//@Comment : 如果idx >= 0表示设置, idx == -1表示取消设置
			int setAffinity(const int idx = 0);
		};//class Thread
	}//namespace thread
}//namespace utils

#endif//UTILS_THREAD_THREAD_H
