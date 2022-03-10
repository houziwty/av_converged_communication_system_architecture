#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "log_host_server.h"

LogHostServer::LogHostServer(FileLog& log)
    : XMQNode(), fileLog{log}
{}

LogHostServer::~LogHostServer()
{}

void LogHostServer::afterFetchOnlineStatusNotification(const bool online)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch log host service online status = [ %d ].", online);
}

void LogHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq host service capabilities size = [ %d ].", number);

    for(int i = 0; i != number; ++i)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", infos[i].name);
    }
}

void LogHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Polled forward data = [ %s ] from server = [ %s ].", 
        msg.c_str(), 
        from);

    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("config"))
        {
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Parsed unknown data = [ %s ] from server = [ %s ].",  
                msg.c_str(), 
                from);
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed forward data = [ %s ] from server = [ %s ] failed, result = [ %d ].",  
            msg.c_str(), 
            from, 
            ret);
    }
}
