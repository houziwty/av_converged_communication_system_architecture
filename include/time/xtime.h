//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 时钟
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_UTILS_TIME_OPERATE_H
#define FRAMEWORK_UTILS_TIME_OPERATE_H

namespace framework
{
	namespace utils
	{
		namespace time
		{
			class XTime
			{
			public:
				XTime(void);
				~XTime(void);

			public:
				//@Return : 时间间隔，单位：毫秒
				const unsigned long long tickcount(void);

				//@interval [in] : 时间间隔，单位：毫秒
				void sleep(const unsigned long long interval = 1);
			};//class XTime
		}//namespace time
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_TIME_OPERATE_H
