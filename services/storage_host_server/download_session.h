//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-21
//		Description : 存储服务数据下载会话
//
//		History:
//					1. 2022-04-21 由王科威创建
//

#ifndef SERVICE_STORAGE_DOWNLOAD_SESSION_H
#define SERVICE_STORAGE_DOWNLOAD_SESSION_H

#include "boost/atomic.hpp"
#include "libavparser.h"
using namespace module::av::stream;
#include "session.h"

class DownloadSession 
    : public Session, protected Libavparser
{
public:
    //@svr [in] : 服务实例
    //@sid [in] : 会话ID
    DownloadSession(
        Server& svr, 
        const uint32_t sid = 0);
    ~DownloadSession(void);

protected:
    void afterParsedDataNotification(
        const uint32_t id = 0, 
        const void* avpkt = nullptr) override;

public:
    int run(void) override;
    int stop(void) override;
    int input(const void* data = nullptr, const uint64_t bytes = 0) override;
};//class DownloadSession

#endif//SERVICE_STORAGE_DOWNLOAD_SESSION_H
