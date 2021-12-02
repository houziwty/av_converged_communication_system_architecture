//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : 连接设备主机的客户端会话
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef SERVICE_DVS_HOST_SESSION_H
#define SERVICE_DVS_HOST_SESSION_H

#include "network/asio/async_tcp_session.h"
using namespace framework::network::asio;

class DvsHostService;

class DvsHostSession final 
    : public AsyncTcpSession
{
public:
    DvsHostSession(
        boost::asio::ip::tcp::socket& s, 
        DvsHostService& dhs,
        const std::string sid);
    ~DvsHostSession(void);

public:
    int createNew(const int bytes = 1048576) override;

protected:
    void afterFetchExpiredEventNotification(const int e = 0);
    void afterFetchSendEventNotification(const int e = 0, const int bytes = 0) override;
    void asyncFetchReceiveEventNotification(const int e = 0, const void* data = nullptr, const int bytes = 0) override;

private:
    DvsHostService& dvsHostService;
    const std::string sessionId;
    unsigned long long totalSendBytes;
    unsigned long long totalRecvBytes;
};//class DvsHostService

#endif//SERVICE_DVS_HOST_SESSION_H
