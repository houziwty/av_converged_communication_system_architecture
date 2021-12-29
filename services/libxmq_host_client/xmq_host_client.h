//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 消息客户端
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef SERVICE_XMQ_HOST_CLIENT_H
#define SERVICE_XMQ_HOST_CLIENT_H

#include "network/xmq/worker_mode.h"
using namespace framework::network::xmq;
#include "utils/url/url.h"
using namespace framework::utils::url;

class LibXmqHostClient;

class XmqHostClient final 
    : public WorkerMode
{
public:
    XmqHostClient(LibXmqHostClient& client);
    ~XmqHostClient(void);

public:
    int start(
        const void* name = nullptr, 
        const int bytes = 0,  
        const char* ip = nullptr, 
        const unsigned short port = 0) override;
    int stop() override;

protected:
	void afterWorkerPolledDataHandler(const void* data = nullptr, const int bytes = 0) override;

private:
    //定时任务执行线程
	void processTaskOnTimerThread(void);

    //注册应答业务处理
	//@url [in] : 应答URL标识
	void processRegisterResponseMessage(Url& url);

    //查询应答业务处理
	//@url [in] : 应答URL标识
	void processQueryResponseMessage(Url& url);

private:
    LibXmqHostClient& libXmqHostClient;
    std::string xmqHostServiceName;
    unsigned long long registerResponseTimetamp;
    void* thread;
};//class XmqHostClient

#endif//SERVICE_XMQ_HOST_CLIENT_H