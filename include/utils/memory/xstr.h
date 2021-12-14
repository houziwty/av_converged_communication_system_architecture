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
                //字节拷贝
			    //@src [in] : 源
                //@dest [in] : 目的
                //@bytes [in] : 源字节数
                void copy(
                    const char* src = nullptr, 
                    char* dest = nullptr, 
                    const unsigned long long bytes = 0);
            };//class XStr
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XSTR_H