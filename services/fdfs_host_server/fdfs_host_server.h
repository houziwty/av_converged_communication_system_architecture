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

#ifndef SERVICE_FAST_DFS_HOST_SERVER_H
#define SERVICE_FAST_DFS_HOST_SERVER_H

#include "libfilelog.h"
using namespace module::file::log;
#include "libxmq.h"
using namespace module::network::xmq;
#include "libfdfs.h"
using namespace module::file::storage;

class FastDFSHostServer final 
    : public Libxmq, protected Libfdfs
{
public:
    FastDFSHostServer(
        const XMQNodeConf& conf, 
        FileLog& flog, 
        const char* fdfsconf = nullptr);
    ~FastDFSHostServer(void);

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
};//class FastDFSHostServer

#endif//SERVICE_FAST_DFS_HOST_SERVER_H