//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 字符串操作
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MEMORY_XSTR_H
#define FRAMEWORK_UTILS_MEMORY_XSTR_H

#include <stdint.h>

namespace framework
{
	namespace utils
	{
		namespace memory
		{
            class XStr
            {
            public:
                XStr(void);
                ~XStr(void);

            public:
                //拷贝
				//@src [in] : 源数据
				//@bytes [in] : 源大小
				//@Return : 错误码
				//@Comment : 1.数据深拷贝
                //           2.创建的拷贝数据由调用者销毁
                const void* copy(
                    const void* data = nullptr, 
                    const uint64_t bytes = 0);
            };//class XStr
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XSTR_H