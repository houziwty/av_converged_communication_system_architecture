//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : XMS客户端会话
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef SERVICE_XMS_CLIENT_SESSION_H
#define SERVICE_XMS_CLIENT_SESSION_H

#include "libasio/tcp/tcp_session.h"
using namespace module::network::asio;

class LibXmsHostClient;

class XmsClientSession final
    : public TcpSession
{
public:
    XmsClientSession(
        LibXmsHostClient& host, 
        SessionPtr ptr, 
        const std::string sid);
    ~XmsClientSession(void);

protected:
    void fetchSentDataEventNotification(
        const int e = 0, 
        const int bytes = 0) override;
    void fetchReceivedDataEventNotification(
        const int e = 0, 
        const void* data = nullptr, 
        const int bytes = 0) override;

private:
    LibXmsHostClient& libXmsHostClient;
    const std::string sessionID;
};//class XmsClientSession

#endif//SERVICE_XMS_CLIENT_SESSION_H