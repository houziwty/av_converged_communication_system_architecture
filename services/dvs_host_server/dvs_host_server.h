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

#ifndef SERVICE_DVS_HOST_SERVER_H
#define SERVICE_DVS_HOST_SERVER_H

#include "boost/shared_ptr.hpp"
#include "log.h"
using namespace module::file::log;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "utils/lock/rw_lock.h"
using namespace framework::utils::lock;
#include "asio_node.h"
using namespace module::network::asio;
#include "xmq_node.h"
using namespace module::network::xmq;
#include "dvs_node.h"
using namespace module::device::dvs;
#include "dvs_stream_session.h"
#include "utils/map/unordered_map.h"

using DVSStreamSessionPtr = boost::shared_ptr<DvsStreamSession>;
using DVSStreamSessionPtrs = UnorderedMap<const uint32_t, DVSStreamSessionPtr>;

class DvsHostServer final 
    : public XMQNode, protected ASIONode, protected DVSNode
{
public:
    DvsHostServer(FileLog& log);
    ~DvsHostServer(void);

public:
	int run(void) override;
	int stop(void) override;

protected:
	void afterPolledDataNotification(
		const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* from = nullptr) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr, 
		const uint32_t number = 0) override;
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
    void afterPolledRealplayDataNotification(
        const uint32_t id = 0, 
        const int32_t channel = 0, 
        const uint32_t type = 0, 
        const void* data = nullptr, 
        const uint32_t bytes = 0) override;

private:
    //移除超时未更新会话
    void removeExpiredSession(const std::string sid);
    
    //DVS设备业务处理
    //@from [in] : 源ID
	//@requestUrl [in] : 请求URL标识
	void processDvsControlMessage(const std::string from, Url& requestUrl);

private:
    FileLog& fileLog;
    uint32_t deviceNumber;
    uint32_t streamNumber;
    SharedMutex mtx;
    DVSStreamSessionPtrs sessions;
};//class DvsHostServer

#endif//SERVICE_DVS_HOST_SERVER_H