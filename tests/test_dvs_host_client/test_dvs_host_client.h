//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 设备主机客户端测试
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef TEST_DVS_HOST_CLIENT_H
#define TEST_DVS_HOST_CLIENT_H

#include "liblog/log.h"
using namespace module::file::log;
#include "libxmq_host_client/libxmq_host_client.h"

class TestDvsHostClient final 
    : protected LibXmqHostClient
{
public:
    TestDvsHostClient(FileLog& log);
    ~TestDvsHostClient(void);

public:
     int start(
        const std::string name, 
        const std::string ip, 
        const unsigned short port = 0);
    int stop(void);

protected:
    void fetchXmqHostClientOnlineStatusNotification(bool online) override;
    void fetchXmqHostServiceCapabilitiesNotification(const std::vector<std::string> services) override;
	void fetchXmqHostClientReceivedDataNotification(const std::string data) override;

private:
    FileLog& fileLog;
};//class TestDvsHostClient

#endif//TEST_DVS_HOST_CLIENT_H