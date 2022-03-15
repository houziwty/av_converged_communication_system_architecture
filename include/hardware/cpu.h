//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : CPU工具类
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_UTILS_HARDWARE_CPU_H
#define FRAMEWORK_UTILS_HARDWARE_CPU_H

namespace framework
{
	namespace utils
	{
		namespace hardware
		{
			class Cpu
			{
			public:
				Cpu(void);
				~Cpu(void);

				//获取CPU个数
				//@Return : CPU个数
				//@Comment : 无论如何都至少返回1
				const int getCount(void) const;
			};//class Cpu
		}//namespace hardware
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_HARDWARE_CPU_H
