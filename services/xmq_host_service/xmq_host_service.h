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
#include "utils/url/url.h"
using namespace framework::utils::url;

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
	//@name [out] : 服务标识
	//@data [out] : 数据
	void afterSwitcherPolledDataHandler(const std::string name, const std::string data) override;

private:
	//服务注册超时检测线程
	void checkRegisterExpiredOfServiceThread(void);

	//注册业务处理
	//@name [in] : 请求服务名称
	//@requestUrl [in] : 请求URL标识
	void processRegisterMessage(const std::string name, Url& requestUrl);

	//请求业务处理
	//@name [in] : 请求服务名称
	//@requestUrl [in] : 请求URL标识
	//@Comment : 请求业务包括：查询等
	void processRequestMessage(const std::string name, Url& requestUrl);

	//转发
	//@name [in] : 服务名称
	//@data [in] : 数据
	void forwardMessage(const std::string name, const std::string data);

private:
	const std::string serviceName;
	FileLog& fileLog;
	UnorderedMap<std::string, unsigned long long> registeredServices;
	bool stopped;
	void* thread;
};//class XmqHostService

#endif//XMQ_HOST_SERVICE_H
