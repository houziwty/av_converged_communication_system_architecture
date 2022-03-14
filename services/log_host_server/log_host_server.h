//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-09
//		Description : 日志存储接入服务
//
//		History:
//					1. 2022-03-09 由王科威创建
//

#ifndef SERVICE_LOG_HOST_SERVER_H
#define SERVICE_LOG_HOST_SERVER_H

#include "file_log.h"
using namespace module::file::log;
#include "xmq_node.h"
using namespace module::network::xmq;

class LogHostServer final 
    : public XMQNode
{
public:
    LogHostServer(
        const std::string& dir, 
        const uint32_t expire = 0);
    ~LogHostServer(void);

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

private:
    const std::string& fileDir;
    const uint32_t expireDays;
    FileLog fileLog;
};//class LogHostServer

#endif//SERVICE_LOG_HOST_SERVER_H