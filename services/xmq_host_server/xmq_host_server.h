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

#ifndef SERVICE_XMQ_HOST_SERVER_H
#define SERVICE_XMQ_HOST_SERVER_H

#include "xmq_node.h"
using namespace module::network::xmq;
#include "utils/map/unordered_map.h"
#include "utils/url/url.h"
using namespace framework::utils::url;

class XmqHostServer final 
	: public XMQNode
{
public:
	XmqHostServer(const XMQModeConf& conf);
	~XmqHostServer(void);

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
	//服务注册超时检测线程
	void checkRegisterExpiredOfServiceThread(void);

	//注册业务处理
	//@from [in] : 源ID
	//@requestUrl [in] : 请求URL标识
	void processRegisterMessage(const std::string from, Url& requestUrl);

	//请求业务处理
	//@from [in] : 源ID
	//@requestUrl [in] : 请求URL标识
	//@Comment : 请求业务包括：查询等
	void processRequestMessage(const std::string from, Url& requestUrl);

	//转发业务消息
	//@from [in] : 源ID
	//@to [in] : 目的ID
	//@data [in] : 源数据
	void forwardCustomMessage(
		const std::string from, 
		const std::string to, 
		const std::string data);

private:
	const XMQModeConf& modeconf;
	std::string logid;
	UnorderedMap<std::string, uint64_t> registeredServices;
	bool stopped;
	_thread_t expire;
};//class XmqHostServer

#endif//SERVICE_XMQ_HOST_SERVER_H
