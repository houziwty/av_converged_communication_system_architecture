#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
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
    const char* key/* = nullptr*/, 
    const char* value/* = nullptr*/)
{
    return Error_Code_Success;
}

const char* DBRedis::read(
    const char* key/* = nullptr*/)
{
    return nullptr;
}
