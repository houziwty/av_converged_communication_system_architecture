//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-21
//		Description : 实时流视频数据上传会话
//
//		History:
//					1. 2022-04-21 由王科威创建
//

#ifndef SERVICE_STORAGE_UPLOAD_SESSION_H
#define SERVICE_STORAGE_UPLOAD_SESSION_H

#include "boost/atomic.hpp"
#include "libav.h"
using namespace module::av::stream;

class Server;

class UploadSession : protected Libav
{
public:
    //id [in] : 会话ID
    UploadSession(Server& svr, const uint32_t id = 0);
    ~UploadSession(void);

public:
    int run(const uint32_t did = 0, const uint32_t cid = 0);
    int stop(void);
    int input(const void* data = nullptr, const uint64_t bytes = 0);
    inline const uint32_t sessionid(void) const
    {
        return sid;
    }

private:
    void sendRealplayRequestThread(
        const uint32_t did = 0, 
        const uint32_t cid = 0);
    void afterGrabPSFrameDataNotification(
        const uint32_t id = 0, 
        const void* avpkt = nullptr);

private:
    Server& server;
    const uint32_t sid;
    boost::atomic_uint64_t sequence;
    void* realplayThread;
    std::string fileName;
};//class UploadSession

#endif//SERVICE_STORAGE_UPLOAD_SESSION_H