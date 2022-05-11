#include "error_code.h"
#include "media_host_server.h"

MediaHostServer::MediaHostServer(FileLog& log)
    : Libasio(), flog{log}, sessionid{0}
{}

MediaHostServer::~MediaHostServer()
{}

uint32_t MediaHostServer::afterFetchIOAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const int32_t e/* = 0*/)
{
    int sid{0};

    if (!e)
    {
        sid = ++sessionid;
        flog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "接收 [ %s:%u ] 连接成功，会话ID=%u", ip, port, sid);
    }
    else
    {
        flog.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "接收 [ %s:%u ] 连接失败，错误码=%d", ip, port, e);
    }

    if (0 < sid)
    {
        /* code */
    }
    
    return sessionid;
}

uint32_t MediaHostServer::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    return 0;
}

void MediaHostServer::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
}

void MediaHostServer::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}
