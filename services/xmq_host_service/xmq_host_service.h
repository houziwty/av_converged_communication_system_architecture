//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 消息服务主机
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef XMQ_HOST_SERVICE_H
#define XMQ_HOST_SERVICE_H

#include "liblog/log.h"
using namespace module::file::log;
#include "utils/map/unordered_map.h"
#include "network/xmq/switcher_pub_model.h"
using namespace framework::network::xmq;

class XmqHostService final 
	: public SwitcherPubModel
{
public:
	XmqHostService(const std::string name, FileLog& log);
	~XmqHostService(void);

public:
	int start(
		const unsigned short switcherPort = 0, 
		const unsigned short publisherPort = 0, 
		const int hwm = 10) override;
	int stop(void) override;

protected:
	//交换模型数据接收回调函数
	//@appid [out] : APP标识
	//@data [out] : 数据
	void afterSwitcherPolledDataHandler(const std::string appid, const std::string data) override;

private:
	//服务注册超时检测线程
	void checkRegisterExpiredOfServiceThread(void);

private:
	const std::string serviceName;
	FileLog& fileLog;
	UnorderedMap<const std::string, unsigned long long> registeredServices;
	bool stopped;
	void* thread;
};//class XmqHostService

#endif//XMQ_HOST_SERVICE_H
