//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-19
//		Description : 分布式存储服务
//
//		History:
//					1. 2022-04-19 由王科威创建
//

#ifndef SERVICE_STORAGE_HOST_SERVER_H
#define SERVICE_STORAGE_HOST_SERVER_H

#include "boost/atomic.hpp"
#include "boost/shared_ptr.hpp"
#include "libfilelog.h"
using namespace module::file::log;
#include "libxmq.h"
using namespace module::network::xmq;
#include "libasio.h"
using namespace module::network::asio;
#include "libfdfs.h"
using namespace module::file::storage;
#include "map/unordered_map.h"

class Session;
using SessionPtr = boost::shared_ptr<Session>;
using Sessions = UnorderedMap<const uint32_t, SessionPtr>;
using Tasks = UnorderedMap<const uint32_t, SessionPtr>;

class Server final 
    : protected Libxmq, protected Libfdfs, protected Libasio
{
public:
    Server(FileLog& flog);
    ~Server(void);

public:
    int run(
        const XMQNodeConf& conf, 
        const char* fdfsconf = nullptr);
	int stop(void);
    //ASIO数据发送
    int send(
        const uint32_t sid = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0);
    //FDFS上传数据
    int upload(
        const uint32_t sid = 0, 
        const void* avpkt = nullptr, 
        const bool flag = false);

protected:
	void afterPolledXMQDataNotification(
		const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* name = nullptr) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(const char* names = nullptr) override;
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

private:
    void processConfigRequest(
        const char* name = nullptr, 
        const char* req = nullptr);

private:
    FileLog& log;
    uint32_t xid;
    const std::string logid;
    boost::atomic_uint32_t sid;
    Sessions sessions;
    Tasks tasks;
};//class Server

#endif//SERVICE_STORAGE_HOST_SERVER_H