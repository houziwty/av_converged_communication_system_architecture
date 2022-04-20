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

#include "libfilelog.h"
using namespace module::file::log;
#include "libxmq.h"
using namespace module::network::xmq;
#include "libasio.h"
using namespace module::network::asio;
#include "libfdfs.h"
using namespace module::file::storage;

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
    uint32_t afterFetchIOConnectedEventNotification(const int32_t e = 0) override;
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
    FileLog& log;
    uint32_t xid;
    const std::string logid;
};//class Server

#endif//SERVICE_STORAGE_HOST_SERVER_H