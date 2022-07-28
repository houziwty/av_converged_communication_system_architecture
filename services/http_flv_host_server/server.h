//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : HTTP-FLV流媒体分发服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_HTTP_FLV_HOST_SERVER_H
#define SERVICE_HTTP_FLV_HOST_SERVER_H

#include "boost/atomic.hpp"
#include "boost/function.hpp"
#include "libxmq.h"
using namespace module::network::xmq;
#include "libasio.h"
using namespace module::network::asio;
#include "libhttp.h"
using namespace module::network::http;
#include "libavparser.h"
#include "libavpkt.h"
using namespace module::av::stream;
#include "libfilelog.h"
using namespace module::file::log;
#include "map/unordered_map.h"
#include "av/flv/flv-muxer.h"

class Server final 
    : protected Libxmq, protected Libasio, protected Libavparser, protected Libhttp
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
    void afterParsedDataNotification(
        const uint32_t id = 0, 
        const void* avpkt = nullptr) override;
    void afterFetchHttpResponseNotification(
        const uint32_t id = 0, 
        const char* response = nullptr, 
        const bool close = false) override;
    void afterFetchHttpRequestNotification(
        const uint32_t id = 0, 
        const char* url = nullptr, 
        const char* contentType = nullptr, 
        const char* content = nullptr) override;

private:
    //配置消息处理
    void processConfigRequest(
        const char* name = nullptr, 
        const char* req = nullptr);

private:
    FileLog& log;
    uint32_t xid;
    const std::string logid;
    boost::atomic_uint32_t sessionIDNumber;
    UnorderedMap<const uint64_t, std::string> asyncMessages;
    bool connected;
    flv_muxer_t* flv;
};//class Server

#endif//SERVICE_HTTP_FLV_HOST_SERVER_H