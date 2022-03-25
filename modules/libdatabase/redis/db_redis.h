//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : REDIS数据库
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef MODULE_FILE_DATABASE_DB_REDIS_H
#define MODULE_FILE_DATABASE_DB_REDIS_H

#include "boost/shared_ptr.hpp"
#include "sw/redis++/redis++.h"
using Redis = sw::redis::Redis;
using RedisPtr = boost::shared_ptr<Redis>;
#include "Database.h"

namespace module
{
    namespace file
    {
        namespace database
        {
            class DBRedis : public Database
            {
            public:
                DBRedis(void);
                virtual ~DBRedis(void);

            public:
				int connect(
                    const char* ip = nullptr, 
                    const uint16_t port = 0, 
                    const char* passwd = nullptr) override;
				int disconnect(void) override;
                int write(
                    const char* key = nullptr, 
                    const char* value = nullptr) override;
                char* read( 
                    const char* key = nullptr) override;

            private:
                RedisPtr redis;
            };//class DBRedis
        }//namespace database
    }//namespace file
}//namespace module

#endif//MODULE_FILE_DATABASE_DB_REDIS_H