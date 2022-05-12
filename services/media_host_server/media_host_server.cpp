#include "error_code.h"
#include "media_host_server.h"

MediaHostServer::MediaHostServer(FileLog& log)
    : Libasio(), flog{log}, sessionIDNumber{0}
{}

MediaHostServer::~MediaHostServer()
{}

uint32_t MediaHostServer::afterFetchIOAcceptedEventNotification(
    const char* remoteIP/* = nullptr*/, 
    const uint16_t remotePort/* = 0*/, 
    const uint16_t localPort/* = 0*/, 
    const int32_t e/* = 0*/)
{
    int currentSID{0};

    if (!e)
    {
        currentSID = ++sessionIDNumber;
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "接收[ %s:%u ]连接本地端口[ %u ]成功，会话ID=%u", 
            remoteIP, remotePort, localPort, currentSID);
    }
    else
    {
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "接收[ %s:%u ]连接本地端口[ %u ]失败，错误码=%d", 
            remoteIP, remotePort, localPort, e);
    }

    if (0 < currentSID)
    {
        /* code */
    }
    
    return currentSID;
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
