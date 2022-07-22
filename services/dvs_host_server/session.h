//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-11
//		Description : DVS流会话
//
//		History:
//					1. 2022-02-11 由王科威创建
//

#ifndef SERVICE_DVS_STREAM_SESSION_H
#define SERVICE_DVS_STREAM_SESSION_H

#include <vector>
#include <mutex>
#include "boost/atomic.hpp"
#include "libavparser.h"
using namespace module::av::stream;

class Server;

class Session
{
public:
    Session(Server& svr);
    ~Session(void);

public:
    int addTarget(const uint32_t sid = 0);
    int removeTarget(const uint32_t sid = 0);
	int input(
        const void* data = nullptr, 
        const uint64_t bytes = 0);

private:
    Server& server;
    std::mutex mtx;
    std::vector<uint32_t> sids;
    boost::atomic_uint64_t sequence;
};//class Session

#endif//SERVICE_DVS_STREAM_SESSION_H