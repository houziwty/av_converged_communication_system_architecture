//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-17
//		Description : C字符串操作
//
//		History:
//					1. 2022-03-17 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MEMORY_XSTR_H
#define FRAMEWORK_UTILS_MEMORY_XSTR_H

#include <cstdint>

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
				//@src_bytes [in] : 大小
                //@dest [in] : 目标数据
                //@dest_bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 1.数据深拷贝
                //           2.如果src_bytes > dest_bytes，只拷贝dest_bytes大小的数据
                int copy(
                    const char* src = nullptr, 
                    const uint64_t src_bytes = 0, 
                    char* dest = nullptr, 
                    const uint64_t dest_bytes = 0);

                //创建
				//@src [in] : 源数据
				//@bytes [in] : 源大小
				//@Return : 内存
				//@Comment : 创建内存由调用者销毁
                char* alloc(
                    const char* src = nullptr, 
                    const uint64_t bytes = 0);

                //大小
				//@src [in] : 源数据
				//@Return : 大小
                const uint64_t len(
                    const char* src = nullptr);
            };//class XStr
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XSTR_H