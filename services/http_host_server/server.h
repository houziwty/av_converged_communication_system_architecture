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

#include "boost/atomic.hpp"
#include "boost/function.hpp"
#include "libxmq.h"
using namespace module::network::xmq;
#include "libfilelog.h"
using namespace module::file::log;
#include "map/unordered_map.h"

class Server final : protected Libxmq
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

    //DVS设备查询
    //@rep [in,out] ：应答
    void getDvslist(std::string& rep);

    //DVS设备添加
    //@req [in] ：请求
    //@rep [in,out] ：应答
    void addDvs(const std::string& req, std::string& rep);

    //DVS设备删除
    //@req [in] ：请求
    //@rep [in,out] ：应答
    void removeDvs(const std::string& req, std::string& rep);

protected:
    void afterPolledXMQDataNotification(
        const uint32_t id = 0, 
        const void* data = nullptr,  
        const uint64_t bytes = 0, 
        const char* name = nullptr) override;
    void afterFetchOnlineStatusNotification(const bool online = false) override;
    void afterFetchServiceCapabilitiesNotification(const char* names = nullptr) override;

private:
    //HTTP服务运行线程
    void httpWorkerThread(void);

    //加载HTTP控制器
    void loadHttpController(void);

    //配置消息处理
    void processConfigRequest(
        const char* name = nullptr, 
        const char* req = nullptr);

    //同步业务消息处理
    //@id [in] ：请求ID
    //@req [in] ：请求
    //@rep [in,out] ：应答
    //@Return ：错误码
    int syncXMQMessageProcess(
        const uint64_t id, 
        const std::string& req, 
        std::string& rep);

private:
    FileLog& log;
    uint32_t xid;
    const std::string logid;
    boost::atomic_uint32_t sessionIDNumber;
    UnorderedMap<const uint64_t, std::string> asyncMessages;
};//class Server

#endif//SERVICE_HTTP_HOST_SERVER_H