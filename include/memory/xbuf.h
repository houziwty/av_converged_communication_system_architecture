//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-16
//		Description : 数据缓存
//                    -----------------------------------------------------
//                    |                          |          |             |
//                    -----------------------------------------------------
//                    0                      currentPos   currentLen     capacity
//
//		History:
//					1. 05-16 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MEMORY_XBUFFER_H
#define FRAMEWORK_UTILS_MEMORY_XBUFFER_H

namespace framework
{
	namespace utils
	{
		namespace memory
		{
            class XBuffer
            {
            public:
                //@capacity [in] : 缓存大小
                XBuffer(const std::size_t capacity = 3 * 1024 * 1024);
                ~XBuffer(void);

            public:
                //输入
                //@data [in] : 数据
                //@bytes [in] : 大小
                //@Return ：错误码
                int input(
                    const char* data = nullptr, 
                    const std::size_t bytes = 0);

                //获取数据
                //@pos [in] : 数据起始位置
                //@Return : 数据
                inline const char* data(const std::size_t pos = 0) const
                {
                    return 0 < currentLen && pos < currentLen ? buffer + pos : nullptr;
                }

                //获取数据大小
                //@pos [in] : 数据起始位置
                //@Return : 数据大小
                inline const std::size_t bytes(const std::size_t pos = 0) const
                {
                    return 0 < currentLen && pos < currentLen ? currentLen - pos : 0;
                }

                //重置
                inline void reset(void)
                {
                    currentLen = 0;
                }

                //数据移动到首部
                //@pos [in] : 数据起始位置
                //@Return : 错误码
                int move(const std::size_t pos = 0);

            protected:
                char* buffer;
                const std::size_t bufferCapacity;
                std::size_t currentLen;
            };//class XBuffer
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif //FRAMEWORK_UTILS_MEMORY_XBUFFER_H
