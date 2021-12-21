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

#ifndef SERVICE_DVS_HOST_SERVICE_H
#define SERVICE_DVS_HOST_SERVICE_H

#include "boost/shared_ptr.hpp"
#include "log.h"
using namespace module::file::log;
#include "utils/map/unordered_map.h"
#include "tcp/tcp_server.h"
#include "tcp/tcp_session.h"
using namespace module::network::asio;
#include "libxmq_host_client.h"
#include "dvs_host_creator.h"
using namespace module::dvs;
#include "dvs/dvs_host_man.h"
using namespace framework::dvs;
#include "utils/url/url.h"
using namespace framework::utils::url;

using TcpSessionPtr = boost::shared_ptr<TcpSession>;
using DvsHostSessions = UnorderedMap<const std::string, TcpSessionPtr>;

class DvsHostService final 
    : protected LibXmqHostClient, protected TcpServer
{
public:
    DvsHostService(FileLog& log);
    ~DvsHostService(void);

public:
    int start(
        const std::string name, 
        const std::string ip, 
        const unsigned short port = 0);
    int stop(void);

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
    void fetchXmqHostClientOnlineStatusNotification(bool online) override;
    void fetchXmqHostServiceCapabilitiesNotification(const std::vector<std::string> services) override;
	void fetchXmqHostClientReceivedDataNotification(const std::string data) override;
    void fetchAcceptedEventNotification(SessionPtr session, const int e = 0) override;

    //DVS设备业务处理
	//@requestUrl [in] : 请求URL标识
	void processDvsControlMessage(Url& requestUrl);

private:
    FileLog& fileLog;
    DvsHostSessions sessions;
    DvsHostCreator dvsHostCreator;
    DvsHostMan dvsHostMan;
};//class DvsHostService

#endif//SERVICE_DVS_HOST_SERVICE_H