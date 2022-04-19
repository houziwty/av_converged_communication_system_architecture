//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 消息分发-处理服务器
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef SERVICE_XMQ_HOST_SERVER_SERVER_H
#define SERVICE_XMQ_HOST_SERVER_SERVER_H

#include "boost/shared_ptr.hpp"
#include "dispatcher.h"
#include "consumer.h"
using namespace module::network::xmq;
#include "libfilelog.h"
using namespace module::file::log;
#include "map/unordered_map.h"

class Server
{
public:
	Server(FileLog& flog);
	~Server(void);

public:
	int run(const uint16_t port = 0);
	int stop(void);
	int processRegisterRequest(
		const char* name = nullptr, 
		const uint64_t timestamp = 0);
	const std::string processQueryRequest(void);
	inline bool existed(const std::string name)
	{
		return 0 < onlineServices.at(name) ? true : false;
	}
	void catchExceptionOfParsingUrl(
		const void* data = nullptr, 
		const uint64_t bytes = 0, 
		const char* name = nullptr, 
		const int ret = 0);

private:
	void servicesOfflineCheckThread(void);

private:
	FileLog& log;
	Dispatcher dispatcher;
	std::vector<boost::shared_ptr<Consumer>> consumers;
	//<name, timestamp>
	UnorderedMap<std::string, uint64_t> onlineServices;
	bool stopped;
	void* offlineCheckThread;
};//class Server

#endif//SERVICE_XMQ_HOST_SERVER_SERVER_H
