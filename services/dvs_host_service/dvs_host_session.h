//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : 设备主机会话
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef SERVICE_DVS_HOST_SESSION_H
#define SERVICE_DVS_HOST_SESSION_H

#include "libasio/tcp/tcp_session.h"
using namespace module::network::asio;

class DvsHostService;

class DvsHostSession final 
    : public TcpSession
{
public:
    DvsHostSession(
        SessionPtr ptr, 
        DvsHostService& host,
        const std::string sid);
    ~DvsHostSession(void);

public:
    int createNew(const unsigned int bytes = 1048576) override;

protected:
    void fetchSentDataEventNotification(
        const int e = 0, 
        const int bytes = 0) override;
    void fetchReceivedDataEventNotification(
        const int e = 0, 
        const void* data = nullptr, 
        const int bytes = 0) override;

private:
    DvsHostService& dvsHostService;
    const std::string sessionId;
    unsigned long long totalSendBytes;
    unsigned long long totalRecvBytes;
};//class DvsHostService

#endif//SERVICE_DVS_HOST_SESSION_H
