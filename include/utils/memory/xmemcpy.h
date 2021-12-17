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
                void copy(
                    const void* src = nullptr, 
                    void* dest = nullptr, 
                    const unsigned long long bytes = 0);
            };//class XMemory
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XMEMORY_H