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

#ifndef FRAMEWORK_UTILS_MEMORY_XMEMORY_H
#define FRAMEWORK_UTILS_MEMORY_XMEMORY_H

namespace framework
{
	namespace utils
	{
		namespace memory
		{
            class XMemory
            {
            public:
                XMemory(void);
                ~XMemory(void);

            public:
                //直接目标拷贝
				//@src [in] : 源数据
				//@src_bytes [in] : 大小
                //@dest [in] : 目标数据
                //@dest_bytes [in] : 大小
				//@Return : 错误码
				//@Comment : 1.数据深拷贝
                //           2.如果src_bytes > dest_bytes，只拷贝dest_bytes大小的数据
                int copy(
                    const void* src = nullptr, 
                    const int src_bytes = 0, 
                    void* dest = nullptr, 
                    const int dest_bytes = 0);

                //新建目标拷贝
				//@src [in] : 源数据
				//@src_bytes [in] : 大小
				//@Return : 目标数据
				//@Comment : 1.目标数据大小为src_bytes大小
                void* copyNew(
                    const void* src = nullptr, 
                    const int src_bytes = 0);
            };//class XMemory
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XMEMORY_H