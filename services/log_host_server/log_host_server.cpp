#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "log_host_server.h"

LogHostServer::LogHostServer(
    const XMQModeConf& conf, 
    FileLog& flog, 
    const uint32_t expire/* = 0*/)
    : XMQNode(), modeconf{conf}, log{ flog }, expireDays{ expire }
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
                std::string filenames;
                const uint32_t start_time{ (uint32_t)atoi(begin.c_str()) }, stop_time{ (uint32_t)atoi(end.c_str()) };
				boost::filesystem::path path(log.dir());
				boost::filesystem::directory_iterator last;

				for (boost::filesystem::directory_iterator it(path); it != last; ++it)
                {
					if (!boost::filesystem::is_directory(*it))
                    {
                        const std::string filename{ it->path().filename().string() };
                        const uint32_t current_time{ (uint32_t)atoi(filename.c_str())};

                        if (start_time <= current_time && stop_time >= current_time)
                        {
                            filenames += ("&date=" + filename);
                        }
					}
				}

				const std::string msg{
                    (boost::format("info://%s?command=query&timestamp=%s%s") % from % timestamp % filenames).str() };
				XMQNode::send(modeconf.id, msg.c_str(), msg.length());
            }
            else if(!command.compare("fetch"))
            {
                FILE* fd{ nullptr };
                uint64_t fdbytes{0};
                char* text{ nullptr };
                std::string msg;

#ifdef _WINDOWS
                const std::string filename{ std::string(log.dir()) + "\\" + date};
                fopen_s(&fd, filename.c_str(), "rb+");
#else
                const std::string filename{ std::string(log.dir()) + "/" + date };
                fd = fopen(filename.c_str(), "rb+");
#endif//_WINDOWS

                if (fd)
                {
                    fseek(fd, 0, SEEK_END);
                    fdbytes = ftell(fd);
                    text = new(std::nothrow) char[fdbytes];
                    if (text)
                    {
#ifdef _WINDOWS
                        fread_s(text, fdbytes, fdbytes, 1, fd);
#else
                        fread(text, fdbytes, 1, fd);
#endif//_WINDOWS
                        msg = (boost::format("info://%s?command=fetch&timestamp=%s%s") % from % timestamp % text).str();
                    }
                    else
                    {
                        msg = (boost::format("info://%s?command=fetch&timestamp=%s") % from % timestamp).str();
                    }

                    fclose(fd);
                    boost::checked_array_delete(text);
                }

				XMQNode::send(modeconf.id, msg.c_str(), msg.length());
            }
        }
    }
}
