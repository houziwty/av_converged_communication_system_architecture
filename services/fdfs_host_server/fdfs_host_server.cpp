#include "boost/format.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "fdfs_host_server.h"

FastDFSHostServer::FastDFSHostServer(
    const XMQNodeConf& conf, 
    FileLog& flog, 
    const char* fdfsconf/* = nullptr*/)
    : Libxmq(), Libfdfs(), modeconf{conf}, log{ flog }
{
    Libfdfs::init(fdfsconf);
}

FastDFSHostServer::~FastDFSHostServer()
{
    Libfdfs::destroy();
}

void FastDFSHostServer::afterFetchOnlineStatusNotification(const bool online)
{
	log.write(
		online ? SeverityLevel::SEVERITY_LEVEL_INFO : SeverityLevel::SEVERITY_LEVEL_WARNING,
		"Fetch message of online status [ %s ] notification.",
		online ? "online" : "offline");
}

void FastDFSHostServer::afterFetchServiceCapabilitiesNotification(
    const char** names/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    std::string text;

    for (int i = 0; i != number; ++i)
    {
        text += ("[ " + std::string(names[i]) + " ]");
    }

	log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
		"Fetch response message of service table [ %s ] notification.",
        text.c_str());
}

void FastDFSHostServer::afterPolledXMQDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/)
{
    Url requestUrl;
    int ret{requestUrl.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.proto().compare("info"))
        {
            std::string command, severity, begin, end, text, date, timestamp;
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
				else if (!parameters[i].key.compare("date"))
				{
					date = parameters[i].value;
				}
				else if (!parameters[i].key.compare("timestamp"))
				{
                    timestamp = parameters[i].value;
				}
            }

            if(!command.compare("add"))
            {
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
