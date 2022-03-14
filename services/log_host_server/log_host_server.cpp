#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "log_host_server.h"

LogHostServer::LogHostServer(
    const std::string& dir, 
    const uint32_t expire/* = 0*/)
    : XMQNode(), fileDir{dir}, expireDays{expire}
{}

LogHostServer::~LogHostServer()
{}

int LogHostServer::run()
{
    int ret{fileLog.createNew()};
    
    if(Error_Code_Success == ret)
    {
        ret = XMQNode::run();
    }

    return ret;
}

int LogHostServer::stop()
{
    return XMQNode::stop();
}

void LogHostServer::afterFetchOnlineStatusNotification(const bool online)
{
}

void LogHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
}

void LogHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("info"))
        {
            std::string command, severity, begin, end, log;
            const std::vector<ParamItem> parameters{requestUrl.getParameters()};
            for(int i = 0; i != parameters.size(); ++i)
            {
                if (!parameters[i].key.compare("command"))
                {
                    command = parameters[i].value;
                }
                else if (!parameters[i].key.compare("severity"))
                {
                    severity = parameters[i].value;
                }
                else if (!parameters[i].key.compare("log"))
                {
                    log = parameters[i].value;
                }
                else if (!parameters[i].key.compare("begin"))
                {
                    begin = parameters[i].value;
                }
                else if (!parameters[i].key.compare("end"))
                {
                    end = parameters[i].value;
                }
            }

            if(!command.compare("add"))
            {
                const SeverityLevel log_severity{
                    (const SeverityLevel)atoi(severity.c_str())};
                if (SeverityLevel::SEVERITY_LEVEL_INFO == log_severity || 
                    SeverityLevel::SEVERITY_LEVEL_WARNING == log_severity || 
                    SeverityLevel::SEVERITY_LEVEL_ERROR == log_severity)
                {
                    fileLog.write(log_severity, log.c_str());
                }
            }
            else if(!command.compare("query"))
            {

            }
            else if(!command.compare("fetch"))
            {
                
            }
        }
    }
}
