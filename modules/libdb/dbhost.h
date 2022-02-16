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

#ifndef MODULE_DB_DB_HOST_H
#define MODULE_DB_DB_HOST_H

#include <string>

#if defined(WINDOWS)
#ifdef USE_MODULE_DB_HOST
#define DB_HOST_EXPORT __declspec(dllimport)
#else
#define DB_HOST_EXPORT __declspec(dllexport)
#endif//USE_MODULE_DB_HOST
#elif defined(__linux__)
#define DB_HOST_EXPORT
#endif//WINDOWS

namespace module
{
    namespace db
    {
        typedef enum tagDataBaseType_t : int
        {
            DATABASE_TYPE_NONE = 0,
            DATABASE_TYPE_REDIS
        }DataBaseType;

        class DB_HOST_EXPORT DatabaseHost
        {
        public:
            DatabaseHost(void);
            ~DatabaseHost(void);

        public:
            //打开
            //@url [in] : 数据库连接标识
            //@Return : 错误码
            int open(const std::string url);

            //关闭
            //@Return : 错误码
            int close(void);

            //读取
            //@Return : 错误码
            int read();

            //写入
            //@level : 等级
            //@fmt : 格式
            //@... : 参数列表
            //@Return : 错误码
            int write(const SeverityLevel severity, const std::string fmt, ...);
        };//class DatabaseHost
    }//namespace db
}//namespace module

#endif//MODULE_DB_DB_HOST_H