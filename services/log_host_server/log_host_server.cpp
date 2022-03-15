#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "log_host_server.h"

LogHostServer::LogHostServer(
    FileLog& flog, 
    const std::string& dir, 
    const uint32_t expire/* = 0*/)
    : XMQNode(), log{ flog }, fileDir{ dir }, expireDays{ expire }
{}

LogHostServer::~LogHostServer()
{}

void LogHostServer::afterFetchOnlineStatusNotification(const bool online)
{
    SeverityLevel sl{ SeverityLevel::SEVERITY_LEVEL_INFO };
    if (!online)
    {
        sl = SeverityLevel::SEVERITY_LEVEL_WARNING;
    }

	log.write(
		sl,
		"Fetch message of online status [ %s ] notification.",
		online ? "online" : "offline");
}

void LogHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    std::string text;

    for (int i = 0; i != number; ++i)
    {
        text += ("[ " + std::string(infos[i].name) + " ]");
    }

	log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
		"Fetch response message of service table [ %s ] notification.",
        text.c_str());
}

void LogHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    Url requestUrl;
    int ret{requestUrl.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.proto().compare("info"))
        {
            std::string command, severity, begin, end, text;
            const std::vector<Parameter> parameters{requestUrl.parameters()};
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
                    text = parameters[i].value;
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
                SeverityLevel log_severity{
                    (SeverityLevel)atoi(severity.c_str())};
                if (SeverityLevel::SEVERITY_LEVEL_INFO == log_severity || 
                    SeverityLevel::SEVERITY_LEVEL_WARNING == log_severity || 
                    SeverityLevel::SEVERITY_LEVEL_ERROR == log_severity)
                {
                    log.write(log_severity, text.c_str());
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
