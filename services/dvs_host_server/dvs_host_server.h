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

#include "log.h"
using namespace module::file::log;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "tcp/tcp_server.h"
#include "tcp/tcp_session.h"
using namespace module::network::asio;
#include "xmq_node.h"
using namespace module::network::xmq;
#include "dvs/dvs_host_man.h"
using namespace framework::dvs;

using TcpSessionPtr = boost::shared_ptr<TcpSession>;
using DvsHostSessions = UnorderedMap<const std::string, TcpSessionPtr>;

class DvsHostServer final 
    : public XMQNode, protected TcpServer
{
public:
    DvsHostServer(FileLog& log);
    ~DvsHostServer(void);

public:
	int run(void) override;
	int stop(void) override;

    //启动XMS
    //@port [in] : 端口号
    //@Return : 错误码
    int startXMS(const unsigned short port = 0);

    //停止XMS
    //@Return : 错误码
    int stopXMS(void);

protected:
	void afterPolledDataNotification(
		const uint32_t id = 0, 
		const char* name = nullptr, 
		const void* data = nullptr, 
		const uint64_t bytes = 0) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr, 
		const uint32_t number = 0) override;
    void fetchAcceptedEventNotification(
        boost::asio::ip::tcp::socket* so/* = nullptr*/, 
        const int e/* = 0*/) override;

private:
    //移除超时未更新会话
    void removeExpiredSession(const std::string sid);
    
    //DVS设备业务处理
	//@requestUrl [in] : 请求URL标识
	void processDvsControlMessage(Url& requestUrl);

private:
    FileLog& fileLog;
    DvsHostSessions sessions;
    DvsHostCreator dvsHostCreator;
    DvsHostMan dvsHostMan;
};//class DvsHostServer

#endif//SERVICE_DVS_HOST_SERVICE_H