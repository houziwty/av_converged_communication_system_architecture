//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-11
//		Description : 数据消费
//
//		History:
//					1. 2022-04-11 由王科威创建
//

#ifndef SERVICE_XMQ_HOST_SERVER_CONSUMER_H
#define SERVICE_XMQ_HOST_SERVER_CONSUMER_H

#include "libxmq.h"
using namespace module::network::xmq;

class Server;

class Consumer final 
	: public Libxmq
{
public:
	Consumer(Server& svr);
	~Consumer(void);

public:
	int run(const uint32_t id = 0);
	int stop(void);

protected:
	void afterPolledXMQDataNotification(
		const uint32_t id = 0, 
		const void* data = nullptr,  
		const uint64_t bytes = 0, 
		const char* name = nullptr) override;
	void forwardMessage(
		const char* from = nullptr, 
		const char* to = nullptr, 
		const void* data = nullptr, 
		const uint64_t bytes = 0);

private:
	XMQNodeConf modeconf;
	Server& server;
};//class Consumer

#endif//SERVICE_XMQ_HOST_SERVER_CONSUMER_H
