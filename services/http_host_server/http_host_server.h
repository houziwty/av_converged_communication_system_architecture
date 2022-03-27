//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : HTTP服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_HTTP_HOST_SERVER_H
#define SERVICE_HTTP_HOST_SERVER_H

#include "asio_node.h"
using namespace module::network::asio;
#include "http_session_node.h"
using namespace module::network::http;
#include "lock/rw_lock.h"
using namespace framework::utils::lock;

class HttpHostServer final 
    : public ASIONode, protected HttpSessionNode
{
public:
    HttpHostServer(void);
    ~HttpHostServer(void);

protected:
    uint32_t afterFetchAcceptedEventNotification(
        const char* ip = nullptr, 
        const uint16_t port = 0, 
        const int32_t e = 0) override;
    uint32_t afterFetchConnectedEventNotification(const int32_t e = 0) override;
    void afterPolledReadDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterPolledSendDataNotification(
        const uint32_t id = 0, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    int onresponse(
		const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0) override;

private:
    SharedMutex mtx;
    uint32_t sid;
};//class HttpHostServer

#endif//SERVICE_HTTP_HOST_SERVER_H