#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"

Server::Server(
    const XMQNodeConf& conf, FileLog& flog, 
    const uint32_t expire/* = 0*/)
    : Libxmq(), modeconf{conf}, log{ flog }, expireDays{ expire }
{}

Server::~Server()
{}

void Server::afterFetchOnlineStatusNotification(const bool online)
{
    if (online)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Fetch log host server online status [ %s ].", online ? "online" : "offline");

        const std::string msg{"config://dvs222_host_server_log?data={\"command\":\"mec.perception.add\"}"};
        Libxmq::send(modeconf.id, msg.c_str(), msg.length());
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch log host server online status [ %s ].", online ? "online" : "offline");
    }
}

void Server::afterFetchServiceCapabilitiesNotification(const char* names/* = nullptr*/)
{
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
        "Fetch system online names of servers [ %s ].", names);
}

void Server::afterPolledXMQDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/)
{
    Url url;
    int ret{url.parse(data, bytes)};

    try
    {
        if(Error_Code_Success == ret && !url.proto().compare("info"))
        {
            const std::vector<Parameter> params{url.parameters()};

            for (int i = 0; i != params.size(); ++i)
            {
                if (!params[i].key.compare("data"))
                {
                    auto o{boost::json::parse(params[i].value).as_object()};
                    auto command{o.at("command").as_string()};
                    auto severity{o.at("severity").as_int64()};
                    auto text{o.at("text").as_string()};

                    if(!command.compare("add"))
                    {
                        SeverityLevel sl{(SeverityLevel)severity};
                        if (SeverityLevel::SEVERITY_LEVEL_INFO == sl || 
                            SeverityLevel::SEVERITY_LEVEL_WARNING == sl || 
                            SeverityLevel::SEVERITY_LEVEL_ERROR == sl)
                        {
                            log.write(sl, text.c_str());
                        }
                    }
                    else if(!command.compare("query"))
                    {
                        auto begin{o.at("begin").as_string()}; 
                        auto end{o.at("end").as_string()};
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
                                    filenames += (filename + ";");
                                }
                            }
                        }

                        const std::string msg{
                            (boost::format("info://%s?data={\"command\":\"query\",\"files\":%s}") % name % filenames).str() };
                        Libxmq::send(modeconf.id, msg.c_str(), msg.length());
                    }
                    else if(!command.compare("fetch"))
                    {
                        FILE* fd{ nullptr };
                        uint64_t fdbytes{0};
                        char* text{ nullptr };
                        std::string msg;
                        auto date{o.at("date").as_string()};

#ifdef _WINDOWS
                        const std::string filename{ std::string(log.dir()) + "\\" + date.c_str()};
                        fopen_s(&fd, filename.c_str(), "rb+");
#else
                        const std::string filename{ std::string(log.dir()) + "/" + date.c_str() };
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
                                msg = (boost::format("info://%s?data={\"command\":\"fetch\",\"text\":%s}") % name % text).str();
                            }
                            else
                            {
                                msg = (boost::format("info://%s?data={\"command\":\"fetch\",\"text\":""}") % name).str();
                            }

                            fclose(fd);
                            boost::checked_array_delete(text);
                        }

                        Libxmq::send(modeconf.id, msg.c_str(), msg.length());
                    }

                    break;
                }
            }
        }
    }
    catch(...)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR,
            "Catch exception from [ %s ] and data [ %s ].", name, (const char*)data);
    }
}
