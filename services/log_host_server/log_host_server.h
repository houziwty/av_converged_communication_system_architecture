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

#include "log.h"
using namespace module::file::log;
#include "xmq_node.h"
using namespace module::network::xmq;
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;

class LogHostServer final 
    : public XMQNode
{
public:
    LogHostServer(
        FileLog& log, 
        const uint32_t expire = 0, 
        const char* dir = nullptr);
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
    //文件超时处理线程
    void checkFileExpiredThread(void);

private:
    FileLog& fileLog;
    const uint32_t expireDays;
    bool stopped;
    ThreadPool tp;
    void* thread;
    const char* fileDir;
};//class LogHostServer

#endif//SERVICE_LOG_HOST_SERVER_H