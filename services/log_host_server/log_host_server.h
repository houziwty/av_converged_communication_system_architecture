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

#include "libfilelog.h"
using namespace module::file::log;
#include "libxmq.h"
using namespace module::network::xmq;

class LogHostServer final 
    : public Libxmq
{
public:
    LogHostServer(
        const XMQNodeConf& conf, 
        FileLog& flog, 
        const uint32_t expire = 0);
    ~LogHostServer(void);

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

private:
    const XMQNodeConf& modeconf;
    FileLog& log;
    const uint32_t expireDays;
};//class LogHostServer

#endif//SERVICE_LOG_HOST_SERVER_H