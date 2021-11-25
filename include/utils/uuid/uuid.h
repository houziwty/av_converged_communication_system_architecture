//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : UUID生成器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_UTILS_UUID_GENERATOR_H
#define FRAMEWORK_UTILS_UUID_GENERATOR_H

#include <string>

namespace framework
{
	namespace utils
	{
		namespace uuid
		{
			class Uuid
			{
			public:
				Uuid(void);
				~Uuid(void);

			public:
				//创建
				//@Return : UUID
				const std::string createNew(void);
			};//class Uuid
		}//namespace uuid
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_UUID_GENERATOR_H
