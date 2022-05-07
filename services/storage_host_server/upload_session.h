//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-21
//		Description : 存储服务数据上传会话
//
//		History:
//					1. 2022-04-21 由王科威创建
//

#ifndef SERVICE_STORAGE_UPLOAD_SESSION_H
#define SERVICE_STORAGE_UPLOAD_SESSION_H

#include "session.h"

class UploadSession : public Session
{
public:
    //@svr [in] : 服务实例
    //@sid [in] : 会话ID
    //@did [in] : 设备ID
    //@cid [in] : 通道ID
    UploadSession(
        Server& svr, 
        const uint32_t sid = 0, 
        const uint32_t did = 0, 
        const uint32_t cid = 0);
    ~UploadSession(void);

public:
    int run(void) override;
    int stop(void) override;
    int input(const void* data = nullptr, const uint64_t bytes = 0) override;

private:
    void sendRealplayRequestThread(void);

private:
    const uint32_t deviceid;
    const uint32_t channelid;
    //实时流获取线程
    void* thread;
    std::string fileName;
    uint32_t frameNumber;
    //Change file name in every 45000 frames or 30 minutes.
    static const uint32_t maxFrameNumber = 45000;
};//class UploadSession

#endif//SERVICE_STORAGE_UPLOAD_SESSION_H