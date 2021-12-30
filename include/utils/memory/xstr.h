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
                //直接字符拷贝
				//@src [in] : 源数据
				//@bytes [in] : 目标大小
                //@dest [in] : 目标数据
				//@Return : 错误码
				//@Comment : 1.数据深拷贝
                int copy(
                    const char* src = nullptr, 
                    const int bytes = 0, 
                    char* dest = nullptr);

                //新建目标拷贝
				//@data [in] : 源数据
				//@bytes [in] : 大小
				//@Return : 目标数据
                const char* copyNew(
                    const char* data = nullptr, 
                    const int bytes = 0);
            };//class XStr
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_MEMORY_XSTR_H