//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-21
//		Description : 存储服务数据传输会话
//
//		History:
//					1. 2022-04-21 由王科威创建
//

#ifndef SERVICE_STORAGE_SESSION_H
#define SERVICE_STORAGE_SESSION_H

#include "boost/atomic.hpp"

class Server;

class Session
{
public:
    //@svr [in] : 服务实例
    //@sid [in] : 会话ID
    Session(Server& svr, const uint32_t sid = 0);
    virtual ~Session(void);

public:
    virtual int run(void) = 0;
    virtual int stop(void) = 0;
    virtual int input(
        const void* data = nullptr, 
        const uint64_t bytes = 0) = 0;
    inline const uint32_t id(void) const
    {
        return sessionid;
    }

protected:
    Server& server;
    const uint32_t sessionid;
    boost::atomic_uint64_t sequence;
};//class Session

#endif//SERVICE_STORAGE_SESSION_H