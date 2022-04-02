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

#include "boost/atomic.hpp"
#include "boost/shared_ptr.hpp"
#include "url/url.h"
using namespace framework::utils::data;
#include "lock/rw_lock.h"
using namespace framework::utils::lock;
#include "asio_node.h"
using namespace module::network::asio;
#include "xmq_node.h"
using namespace module::network::xmq;
#include "dvs_node.h"
using namespace module::device::dvs;
#include "dvs_stream_session.h"
#include "map/unordered_map.h"

using DVSStreamSessionPtr = boost::shared_ptr<DvsStreamSession>;
using DVSStreamSessionPtrs = UnorderedMap<const uint32_t, DVSStreamSessionPtr>;
using AVFrameIDs = UnorderedMap<const std::string, uint64_t>;

class DvsHostServer final 
    : public XMQNode, protected ASIONode, protected DVSNode
{
public:
    DvsHostServer(const XMQModeConf& conf);
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
    //处理配置请求
    //@from [in] : 源ID
	//@url [in] : 请求URL
	void config(const std::string from, Url& url);

    //新增设备
    //@from [in] : 源ID
    //@json [in] : 设备JSON描述
    //@Return ：错误码
    int add(
        const std::string& from, 
        const std::string& factory, 
        const std::string& name, 
        const std::string& ip, 
        const std::string& port, 
        const std::string& user, 
        const std::string& passwd, 
        const std::string& timestamp);

    //删除设备
    //@from [in] : 源ID
    //@json [in] : 设备ID
    //@Return ：错误码
    int remove(
        const std::string& from, 
        const std::string& id, 
        const std::string& timestamp);

    //查询设备集合
    //@from [in] : 源ID
    //@json [in] : 设备JSON描述
    //@Return ：错误码
    int query(const std::string& from, const std::string& json);

public:
    const XMQModeConf& modeconf;

private:
    const std::string logid;
    boost::atomic_int deviceNumber;
    uint32_t streamNumber;
    SharedMutex mtx;
    DVSStreamSessionPtrs sessions;
    AVFrameIDs fids;
};//class DvsHostServer

#endif//SERVICE_DVS_HOST_SERVER_H