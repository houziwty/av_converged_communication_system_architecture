//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 消息接入
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef SERVICE_XMQ_HOST_CLIENT_H
#define SERVICE_XMQ_HOST_CLIENT_H

#include "network/xmq/worker_model.h"
using namespace framework::network::xmq;

class XmqHostClient final 
    : public WorkerModel
{
public:
    XmqHostClient(const std::string name);
    ~XmqHostClient(void);

public:
    //查询在线服务
    //@Return : 错误码
    int queryOnlineService(void);

protected:
	void afterWorkerPolledDataHandler(const std::string data) override;

private:
    const std::string xmqHostName;
};//class XmqHostClient

#endif//SERVICE_XMQ_HOST_CLIENT_H