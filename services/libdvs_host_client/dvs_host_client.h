//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 设备主机接入客户端
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef DVS_HOST_CLIENT_H
#define DVS_HOST_CLIENT_H

//#include "boost/shared_ptr.hpp"
// #include "network/asio/async_tcp_server.h"
// using namespace framework::network::asio;
#include "network/xmq/worker_deal.h"
using namespace framework::network::xmq;

class DvsHostClient final 
    : public WorkerDeal//, protected AsyncTcpServer
{
public:
    DvsHostClient(void);
    ~DvsHostClient(void);

public:
    int start(
       const std::string appid, 
       const std::string xmqid, 
       const std::string ip, 
       const unsigned short port = 0) override;
    int stop(void) override;

protected:
	void afterWorkerPolledDataHandler(const std::string data) override;
};//class DvsHostClient

#endif//DVS_HOST_CLIENT_H