//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-11
//		Description : 数据分发
//
//		History:
//					1. 2022-04-11 由王科威创建
//

#ifndef SERVICE_XMQ_HOST_SERVER_DISPATCHER_H
#define SERVICE_XMQ_HOST_SERVER_DISPATCHER_H

#include "libxmq.h"
using namespace module::network::xmq;

class Dispatcher final 
	: public Libxmq
{
public:
	Dispatcher(void);
	~Dispatcher(void);

public:
	int run(const uint16_t port = 0);
	int stop(void);

private:
	XMQNodeConf modeconf;
};//class Dispatcher

#endif//SERVICE_XMQ_HOST_SERVER_DISPATCHER_H
