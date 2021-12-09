//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : HTTP-FLV流会话
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef SERVICE_HTTP_FLV_STREAM_SESSION_H
#define SERVICE_HTTP_FLV_STREAM_SESSION_H

#include "libasio/tcp/http/http_session.h"
using namespace module::network::asio;

class HttpFlvStreamSession final 
    : public HttpSession
{
public:
    HttpFlvStreamSession(SessionPtr ptr);
    ~HttpFlvStreamSession(void);

protected:
    void fetchSentDataEventNotification(
        const int e = 0, 
        const int bytes = 0) override;
    void fetchReceivedDataEventNotification(
        const int e = 0, 
        const void* data = nullptr, 
        const int bytes = 0) override;
};//class HttpFlvStreamSession

#endif//SERVICE_HTTP_FLV_STREAM_SESSION_H
