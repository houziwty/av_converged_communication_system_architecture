//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : 流媒体服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_MEDIA_HOST_SERVER_H
#define SERVICE_MEDIA_HOST_SERVER_H

#include "boost/atomic.hpp"
#include "libasio.h"
using namespace module::network::asio;
#include "libfilelog.h"
using namespace module::file::log;

class MediaHostServer final 
    : public Libasio
{
public:
    MediaHostServer(FileLog& log);
    ~MediaHostServer(void);

protected:
    uint32_t afterFetchIOAcceptedEventNotification(
        const char* ip = nullptr, 
        const uint16_t port = 0, 
        const int32_t e = 0) override;
    uint32_t afterFetchIOConnectedEventNotification(
        const int32_t e = 0, 
        void* user = nullptr) override;
    void afterPolledIOReadDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterPolledIOSendDataNotification(
        const uint32_t id = 0, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;

private:
    FileLog& flog;
    boost::atomic_uint32_t sessionid;
};//class MediaHostServer

#endif//SERVICE_MEDIA_HOST_SERVER_H