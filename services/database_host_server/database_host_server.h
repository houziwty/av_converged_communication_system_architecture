//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : 数据库接入服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_DATABASE_HOST_SERVER_H
#define SERVICE_DATABASE_HOST_SERVER_H

#include "libfilelog.h"
using namespace module::file::log;
#include "libxmq.h"
using namespace module::network::xmq;
#include "database.h"
using namespace module::file::database;
#include "url/url.h"
using namespace framework::utils::data;

class DatabaseHostServer final 
    : public XMQNode, protected DatabaseNode
{
public:
    DatabaseHostServer(const XMQModeConf& conf);
    ~DatabaseHostServer(void);

public:
	int run(
        const DatabaseType& type = DatabaseType::DATABASE_TYPE_NONE, 
        const char* ip = nullptr, 
        const char* auth = nullptr, 
        const uint16_t port = 0);
	int stop(void) override;

protected:
	void afterPolledDataNotification(
		const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* from = nullptr) override;
	void afterFetchOnlineStatusNotification(const bool online = false) override;
	void afterFetchServiceCapabilitiesNotification(const char* names = nullptr) override;

private:    
    //数据配置
    //@from [in] : 源ID
	//@url [in] : 请求URL
	void config(const std::string from, Url& url);

private:
    const std::string logid;
    FileLog log;
    const XMQModeConf& modeconf;
    uint32_t id;
};//class DatabaseHostServer

#endif//SERVICE_DATABASE_HOST_SERVER_H