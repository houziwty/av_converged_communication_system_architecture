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
#include "liblog/log.h"
using namespace module::file::log;
#include "utils/map/unordered_map.h"
#include "libasio/tcp/tcp_server.h"
#include "libasio/tcp/tcp_session.h"
using namespace module::network::asio;
#include "network/xmq/worker_model.h"
using namespace framework::network::xmq;

using TcpSessionPtr = boost::shared_ptr<TcpSession>;
using DvsHostSessions = UnorderedMap<const std::string, TcpSessionPtr>;

class DvsHostService final 
    : public WorkerModel, protected TcpServer
{
public:
    DvsHostService(FileLog& log);
    ~DvsHostService(void);

public:
    int start(
       const std::string appid, 
       const std::string xmqid, 
       const std::string ip, 
       const unsigned short port = 0) override;
    int stop(void) override;

    //移除超时未更新会话
    void removeExpiredSession(const std::string sid);

    //启动XMS
    //@port [in] : 端口号
    //@Return : 错误码
    int startXMS(const unsigned short port = 0);

    //停止XMS
    //@Return : 错误码
    int stopXMS(void);

protected:
	void afterWorkerPolledDataHandler(const std::string data) override;
    void fetchAcceptedEventNotification(SessionPtr session, const int e = 0) override;

private:
    FileLog& fileLog;
    DvsHostSessions sessions;
};//class DvsHostService

#endif//DVS_HOST_SERVICE_H