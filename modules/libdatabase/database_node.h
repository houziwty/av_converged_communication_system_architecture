//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-17
//		Description : 数据库节点
//
//		History:
//					1. 2022-03-17 由王科威创建
//

#ifndef MODULE_FILE_DATABASE_DATABASE_NODE_H
#define MODULE_FILE_DATABASE_DATABASE_NODE_H

#include "defs.h"

namespace module
{
    namespace file
    {
        namespace database
        {
            class FILE_DATABASE_EXPORT DatabaseNode
            {
            public:
                DatabaseNode(void);
                virtual ~DatabaseNode(void);

            public:
                //添加数据库
                //@conf [in] : 数据库配置
                //@Return : 错误码
                int addConf(const DBModeConf& conf);

                //删除数据库
                //@id [in] : 数据库ID
                //@Return : 错误码
                int removeConf(const uint32_t id = 0);

                //运行数据库
				//@id [in] : 数据库ID
				//@Return : 错误码
				virtual int run(const uint32_t id = 0);

				//停止数据库
				//@id [in] : 数据库ID
				//@Return : 错误码
				virtual int stop(const uint32_t id = 0);

                //写入数据
                //@id [in] : 数据库ID
                //@key [in] : 字段标识
                //@value [in] : 字段值
                //@Return : 错误码
                int write(
                    const uint32_t id = 0, 
                    const char* key = nullptr, 
                    const char* value = nullptr);

                //读取数据
                //@id [in] : 数据库ID
                //@key [in] : 字段标识
                //@Return : 字段值
                const char* read(
                    const uint32_t id = 0, 
                    const char* key = nullptr);
            };//class DatabaseHost
        }//namespace database
    }//namespace file
}//namespace module

#endif//MODULE_FILE_DATABASE_DATABASE_NODE_H