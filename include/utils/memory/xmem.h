//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 内存操作
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MEMORY_XMEM_H
#define FRAMEWORK_UTILS_MEMORY_XMEM_H

#include <stdint.h>

namespace framework
{
	namespace utils
	{
		namespace memory
		{
            class XMem
            {
            public:
                XMem(void);
                ~XMem(void);

            public:
                //内存拷贝
				//@src [in] : 源数据
				//@src_bytes [in] : 大小
                //@dest [in] : 目标数据
                //@dest_bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 1.数据深拷贝
                //           2.如果src_bytes > dest_bytes，只拷贝dest_bytes大小的数据
                int copy(
                    const void* src = nullptr, 
                    const uint64_t src_bytes = 0, 
                    void* dest = nullptr, 
                    const uint64_t dest_bytes = 0);

                //创建内存并拷贝
				//@src [in] : 源数据
				//@bytes [in] : 源大小
				//@Return : 创建内存
				//@Comment : 创建内存由调用者销毁
                void* copyNew(
                    const void* src = nullptr, 
                    const uint64_t bytes = 0);

                //内存移动
                //@src [in] : 源数据
                //@dest [in] : 目标数据
                //@dest_bytes [in] : 大小
                //@Return : 错误码
                int move(
                    const void* src = nullptr, 
                    void* dest = nullptr, 
                    const uint64_t dest_bytes = 0);
            };//class XMem
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XMEM_H