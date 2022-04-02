#include <unordered_set>
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "db_redis.h"
using namespace module::file::database;

DBRedis::DBRedis() : Database()
{}

DBRedis::~DBRedis()
{}

int DBRedis::connect(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const char* passwd/* = nullptr*/)
{
    sw::redis::ConnectionOptions connection;
    connection.host.append(ip);
    connection.port = static_cast<int>(port);
    connection.password.append(passwd);
    // Optional. Timeout before we successfully send request to or receive response from redis.
    // By default, the timeout is 0ms, i.e. never timeout and block until we send or receive successfuly.
    // NOTE: if any command is timed out, we throw a TimeoutError exception.
    connection.socket_timeout = std::chrono::milliseconds(200);

    sw::redis::ConnectionPoolOptions pool;
    pool.size = 10;
    // Optional. Max time to wait for a connection. 0ms by default, which means wait forever.
    // Say, the pool size is 3, while 4 threds try to fetch the connection, one of them will be blocked.
    pool.wait_timeout = std::chrono::milliseconds(100);
    // Optional. Max lifetime of a connection. 0ms by default, which means never expire the connection.
    // If the connection has been created for a long time, i.e. more than `connection_lifetime`,
    // it will be expired and reconnected.
    pool.connection_lifetime = std::chrono::minutes(10);

    // Connect to Redis server with a connection pool.
    boost::shared_ptr<Redis> ptr{
        boost::make_shared<Redis>(connection, pool)};
    if (ptr)
    {
        redis.swap(ptr);
    }

    return redis ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int DBRedis::disconnect()
{
    if (redis)
    {
        redis.reset();
    }
    
    return redis ? Error_Code_Operate_Failure : Error_Code_Success;
}

int DBRedis::write(
    const char* transaction/* = nullptr*/)
{
    int ret{transaction ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        try
        {
            std::vector<std::string> items;
            boost::split(items, transaction, boost::is_any_of(" "));
            redis->command<void>(items.begin(), items.end());
        }
        catch (const sw::redis::ReplyError& e) 
        {
            // WRONGTYPE Operation against a key holding the wrong kind of value
            ret = Error_Code_Invalid_Value_Of_Key;
        } 
        catch (const sw::redis::TimeoutError& e) 
        {
            // reading or writing timeout
            ret = Error_Code_Operate_Timeout;
        } 
        catch (const sw::redis::ClosedError& e) 
        {
            // the connection has been closed.
            ret = Error_Code_Invalid_Connection;
        } 
        catch (const sw::redis::IoError& e) 
        {
            // there's an IO error on the connection.
            ret = Error_Code_IO_Error_Of_Connection;
        } 
        catch (const sw::redis::Error& e) 
        {
            // other errors
        }
    }

    return ret;
}

char* DBRedis::read(
    const char* transaction/* = nullptr*/)
{
    int ret{transaction ? Error_Code_Success : Error_Code_Invalid_Param};
    char* buf{nullptr};

    if (Error_Code_Success == ret)
    {
        try
        {
            std::vector<std::string> items;
            boost::split(items, transaction, boost::is_any_of(" "));
            auto out = redis->command(items.begin(), items.end());

            if (out.get()->str)
            {
                buf = XStr().alloc(out.get()->str, out.get()->len);
            }
            else
            {
                std::string temp;
                for (int i = 0; i != out.get()->elements;)
                {
                    if (out.get()->element[i + 1]->str)
                    {
                        temp.append(out.get()->element[i + 1]->str);
                        temp.append("_");
                    }

                    i += 2;
                }
                
                buf = XStr().alloc(temp.c_str(), temp.length() - 1);
            }
        }
        catch (const sw::redis::ReplyError& e) 
        {
            // WRONGTYPE Operation against a key holding the wrong kind of value
            ret = Error_Code_Invalid_Value_Of_Key;
        } 
        catch (const sw::redis::TimeoutError& e) 
        {
            // reading or writing timeout
            ret = Error_Code_Operate_Timeout;
        } 
        catch (const sw::redis::ClosedError& e) 
        {
            // the connection has been closed.
            ret = Error_Code_Invalid_Connection;
        } 
        catch (const sw::redis::IoError& e) 
        {
            // there's an IO error on the connection.
            ret = Error_Code_IO_Error_Of_Connection;
        } 
        catch (const sw::redis::Error& e) 
        {
            // other errors
            ret = Error_Code_DB_Other_Error;
        }
    }

    return buf;
}
