//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-15
//		Description : 任意字节流
//
//		History:
//					1. 2022-03-15 由王科威创建
//

#ifndef FRAMEWORK_UTILS_DATA_ANY_H
#define FRAMEWORK_UTILS_DATA_ANY_H

#include <stdint.h>

namespace framework
{
    namespace utils
    {
        namespace data
        {
            class Any
            {
            public:
                Any(void);
                virtual ~Any(void);

            public:
                //输入数据
                //@data [in] : 数据
                //@bytes [in] : 大小
                //@Return : 错误码
                int input(const void* data = nullptr, const uint64_t bytes = 0);

                //获取数据
                //@Return : 数据
                inline const void* data(void) const
                {
                    return _data;
                }

                //获取数据大小
                //@Return : 数据大小
                inline const uint64_t bytes(void) const
                {
                    return _bytes;
                }

            private:
                void* _data;
                uint64_t _bytes;
            };//class Any
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_DATA_ANY_H