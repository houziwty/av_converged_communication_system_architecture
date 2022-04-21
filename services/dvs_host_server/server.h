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
#include "libfilelog.h"
using namespace module::file::log;
#include "libasio.h"
using namespace module::network::asio;
#include "libxmq.h"
using namespace module::network::xmq;
#include "libdvs.h"
using namespace module::device::dvs;
#include "libavparser.h"
using namespace module::av::stream;
#include "session.h"
#include "map/unordered_map.h"

using SessionPtr = boost::shared_ptr<Session>;
using StreamSessions = UnorderedMap<const std::string, SessionPtr>;

class Server final 
    : protected Libxmq, protected Libasio, protected Libdvs, protected Libavparser
{
public:
    Server(FileLog& flog);
    ~Server(void);

public:
	int run(const XMQNodeConf& conf);
	int stop(void);
    int sendframe(
        const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0);

protected:
	void afterPolledXMQDataNotification(
		const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* name = nullptr) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(
		const char** names = nullptr, 
		const uint32_t number = 0) override;
    uint32_t afterFetchIOAcceptedEventNotification(
        const char* ip = nullptr, 
        const uint16_t port = 0, 
        const int32_t e = 0) override;
    uint32_t afterFetchIOConnectedEventNotification(
        const int32_t e = 0, 
        void* user = nullptr) override;
    void afterPolledIOReadDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterPolledIOSendDataNotification(
        const uint32_t id = 0, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterPolledDVSDataNotification(
        const uint32_t id = 0, 
        const int32_t channel = 0, 
        const uint32_t type = 0, 
        const void* data = nullptr, 
        const uint32_t bytes = 0) override;
    void afterPolledDVSExceptionNotification(
        const uint32_t id = 0, 
        const int32_t error = 0) override;
    void afterParsedDataNotification(
        const uint32_t id = 0, 
        const void* avpkt = nullptr) override;

private:
	void processConfigRequest(
        const char* name = nullptr, 
        const char* req = nullptr);
    int addDVS(
        const std::string& factory, 
        const std::string& ip, 
        const std::string& port, 
        const std::string& user, 
        const std::string& passwd, 
        const std::string& from, 
        const std::string& name, 
        const std::string& timestamp);
    int removeDVS(
        const std::string& from, 
        const std::string& id, 
        const std::string& timestamp);

private:
    FileLog& log;
    uint32_t xid;
    const std::string logid;
    boost::atomic_int did;
    boost::atomic_uint32_t sid;
    StreamSessions sss;
};//class Server

#endif//SERVICE_DVS_HOST_SERVER_H