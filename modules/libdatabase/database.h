//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-17
//		Description : 数据库
//
//		History:
//					1. 2022-03-17 由王科威创建
//

#ifndef MODULE_FILE_DATABASE_DATABASE_H
#define MODULE_FILE_DATABASE_DATABASE_H

#include "defs.h"

namespace module
{
    namespace file
    {
        namespace database
        {
            class Database
            {
            public:
                Database(void);
                virtual ~Database(void);

            public:
                //连接数据库
				//@ip [in] : IP地址
                //@port [in] : 端口号
                //@passwd [in] : 密码
				//@Return : 错误码
				virtual int connect(
                    const char* ip = nullptr, 
                    const uint16_t port = 0,  
                    const char* passwd = nullptr) = 0;

				//断开数据库
				//@Return : 错误码
				virtual int disconnect(void) = 0;

                //写入数据
                //@id [in] : 数据库ID
                //@key [in] : 字段标识
                //@value [in] : 字段值
                //@Return : 错误码
                virtual int write(
                    const char* key = nullptr, 
                    const char* value = nullptr) = 0;

                //读取数据
                //@key [in] : 字段标识
                //@Return : 字段值
                virtual char* read( 
                    const char* key = nullptr) = 0;
            };//class Database
        }//namespace database
    }//namespace file
}//namespace module

#endif//MODULE_FILE_DATABASE_DATABASE_H