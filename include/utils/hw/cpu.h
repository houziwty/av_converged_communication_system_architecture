//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					CPU工具类
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef UTILS_HW_CPU_H
#define UTILS_HW_CPU_H

#include <stddef.h>

namespace utils
{
	namespace hw
	{
		class Cpu
		{
		public:
			Cpu(void);
			~Cpu(void);

			//获取CPU个数
			//@Return : CPU个数
			//@Comment : 无论如何都至少返回1
			const size_t getCount(void) const;
		};//class Cpu
	}//namespace hw
}//namespace utils

#endif//UTILS_HW_CPU_H
