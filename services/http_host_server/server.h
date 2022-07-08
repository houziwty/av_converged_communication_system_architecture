//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : HTTP Web服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_HTTP_HOST_SERVER_H
#define SERVICE_HTTP_HOST_SERVER_H

#include <unordered_map>
#include "boost/atomic.hpp"
#include "boost/function.hpp"
#include "libasio.h"
using namespace module::network::asio;
#include "libhttp.h"
using namespace module::network::http;
#include "libxmq.h"
using namespace module::network::xmq;
#include "libfilelog.h"
using namespace module::file::log;

//HTTP请求回调
//@_1 [in] : 参数
//@_2 [in, out] : HTTP错误码
//@_3 [in, out] : 消息体
//@_4 [in, out] : 消息体类型
typedef boost::function<void(const char*, int&, char*&, char*&)> AfterFetchHttpRequestCallback;

class Server final 
    : protected Libxmq, protected Libasio, protected Libhttp
{
public:
    Server(FileLog& flog);
    ~Server(void);

public:
    //运行
    //@Return ：错误码
    int run(const XMQNodeConf& conf);

    //停止
    //@Return ：错误码
    int stop(void);

protected:
    void afterPolledXMQDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* name = nullptr) override;
    void afterFetchOnlineStatusNotification(const bool online = false) override;
    void afterFetchServiceCapabilitiesNotification(const char* names = nullptr) override;
    uint32_t afterFetchIOAcceptedEventNotification(
        const char* remoteIP = nullptr, 
        const uint16_t remotePort = 0, 
        const uint16_t localPort = 0, 
        const int32_t e = 0) override;
    uint32_t afterFetchIOConnectedEventNotification(
        const int32_t e = 0, 
        void* user = nullptr) override;
    void afterPolledIOReadDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterPolledIOSendDataNotification(
        const uint32_t id = 0, 
        const uint64_t bytes = 0, 
        const int32_t e = 0) override;
    void afterFetchHttpResponseNotification(
        const uint32_t id = 0, 
        const char* response = nullptr,  
        const bool close = false) override;
    void afterFetchHttpRequestNotification(
        const uint32_t id, 
        const char* url, 
        int& e, 
        char*& body, 
        char*& type) override;

private:
    //@params [in] : 参数
    //@e [in/out] ：HTTP错误码
    //@content [in/out] : 消息体
    //@type [in/out] : 消息体类型
    // /api/v1/getapilist处理
    void getAPIList(const char* params, int& e, char*& content, char*& type);
    // /api/v1/device/add处理
    void addDevice(const char* params, int& e, char*& content, char*& type);
    // /api/v1/device/remove处理
    void removeDevice(const char* params, int& e, char*& content, char*& type);

    //加载API接口列表
    //@Return ：错误码
    int loadHttpAPIList(void);

    void processConfigRequest(
        const char* name = nullptr, 
        const char* req = nullptr);

private:
    FileLog& log;
    uint32_t xid;
    const std::string logid;
    boost::atomic_uint32_t sessionIDNumber;
    //API调用回调集合
    std::unordered_map<std::string, std::pair<AfterFetchHttpRequestCallback, std::string>> callbacks;
};//class Server

#endif//SERVICE_HTTP_HOST_SERVER_H