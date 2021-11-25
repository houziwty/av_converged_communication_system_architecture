//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 设备主机接入服务
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef DVS_HOST_SERVICE_H
#define DVS_HOST_SERVICE_H

#include "boost/shared_ptr.hpp"
#include "utils/map/unordered_map.h"
#include "avcap/av_capture.h"
using namespace framework::media::av;
#include "network/asio/async_tcp_server.h"
using namespace framework::network::asio;
#include "network/xmq/worker_deal.h"
using namespace framework::network::xmq;

class DvsHostSession;
using DvsHostSessionPtr = boost::shared_ptr<DvsHostSession>;
using Sessions = UnorderedMap<const std::string, DvsHostSessionPtr>;
 using AVCapturePtr = boost::shared_ptr<AVCapture>;

class DvsHostService final 
    : public WorkerDeal, protected AsyncTcpServer
{
public:
    //@port [in] : 流媒体监听端口号
    DvsHostService(const unsigned short port = 10000);
    ~DvsHostService(void);

public:
    int start(
        const std::string uid, 
        const std::string ip, 
        const unsigned short port = 0) override;
    int stop(void) override;
    void removeExpiredSession(const std::string sid);

protected:
	void afterWorkerPolledDataHandler(const std::string data) override;
    void afterFetchAsyncAcceptEventNotification(boost::asio::ip::tcp::socket& s) override;

private:
    const unsigned short xmsPort;
    AVCapturePtr capturePtr;
    Sessions sessions;
};//class DvsHostService

#endif//DVS_HOST_SERVICE_H