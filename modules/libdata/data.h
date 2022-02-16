//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-14
//		Description : 数据库主机
//
//		History:
//					1. 2021-12-14 由王科威创建
//

#ifndef MODULE_COMMON_DATA_DATA_H
#define MODULE_COMMON_DATA_DATA_H

#include "defs.h"

namespace module
{
    namespace common
    {
        namespace data
        {
            class COMMON_DATA_EXPORT Data
            {
            public:
                Data(void);
                virtual ~Data(void);

            public:
                //输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
                //@Common : 重复输入数据会覆盖
                int input(
					const uint8_t* data = nullptr, 
					const uint64_t bytes = 0);

                //添加下一个
                int add(Data* next = nullptr);

                //获取数据
                //@Return : 数据
                inline const uint8_t* _data(void) const
                {
                    return data;
                }

                //获取数据大小
                //@Return : 数据大小
                inline const uint64_t _bytes(void) const
                {
                    return bytes;
                }

            private:
                uint8_t* data;
                uint64_t bytes;
                Data* next;
            };//class Data
        }//namespace data
    }//namespace common
}//namespace module

#endif//MODULE_COMMON_DATA_DATA_H