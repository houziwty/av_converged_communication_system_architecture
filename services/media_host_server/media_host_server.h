//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-18
//		Description : 流媒体服务
//
//		History:
//					1. 2022-03-18 由王科威创建
//

#ifndef SERVICE_MEDIA_HOST_SERVER_H
#define SERVICE_MEDIA_HOST_SERVER_H

#include "boost/atomic.hpp"
#include "libasio.h"
using namespace module::network::asio;
#include "libhttp.h"
using namespace module::network::http;
#include "libfilelog.h"
using namespace module::file::log;
#include "map/unordered_map.h"

class MediaHostServer final 
    : public Libasio, protected Libhttp
{
public:
    MediaHostServer(FileLog& log);
    ~MediaHostServer(void);

public:
    //添加协议
    //@name : 协议名称
    //@port : 端口号
    //@Return ：错误码
    //@Comment ：服务接收远程连接时通过端口确定协议类型
    int addProtoSupport(const std::string name, const uint16_t port = 0);

protected:
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
        const void* data = nullptr, 
        const uint64_t bytes = 0, 
        const bool close = false) override;

private:
    //创建会话
    int createNewSession(const uint16_t port = 0, const uint32_t id = 0);

private:
    FileLog& flog;
    boost::atomic_uint32_t sessionIDNumber;
    //<端口号,协议名称>
    UnorderedMap<const uint16_t, std::string> protos;
    //<会话ID,协议名称>
    UnorderedMap<const uint32_t, std::string> sessions;
};//class MediaHostServer

#endif//SERVICE_MEDIA_HOST_SERVER_H