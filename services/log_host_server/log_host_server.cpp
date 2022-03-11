#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/filesystem.hpp"
#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "log_host_server.h"

LogHostServer::LogHostServer(
    const std::string& dir, 
    const uint32_t expire/* = 0*/)
    : XMQNode(), fileDir{dir}, expireDays{expire}, thread{nullptr}, stopped{false}
{}

LogHostServer::~LogHostServer()
{}

int LogHostServer::run()
{
    //The dirctory must exist before starting XMQ connection.
    int ret{createDir()};

    if (Error_Code_Success == ret)
    {
        thread = tp.createNew(boost::bind(&LogHostServer::checkFileExpiredThread, this));
        ret = XMQNode::run();
    }
    
    return ret;
}

int LogHostServer::stop()
{
    stopped = true;
    tp.destroy(thread);
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
            std::string level, content;
            const std::vector<ParamItem> parameters{requestUrl.getParameters()};
            for(int i = 0; i != parameters.size(); ++i)
            {
                if (!parameters[i].key.compare("level"))
                {
                    level = parameters[i].value;
                }
                else if (!parameters[i].key.compare("content"))
                {
                    content = parameters[i].value;
                }
            }

            const int logSeverity{atoi(level.c_str())};
            if (0 == logSeverity || 1 == logSeverity || 2 == logSeverity)
            {
                fileLog.write((SeverityLevel)logSeverity, content.c_str());
            }
        }
    }
}

int LogHostServer::createDir()
{
    int ret{Error_Code_Directory_Not_Exist};

    if (boost::filesystem::exists(""))
    {
        boost::filesystem::create_directory(fileDir);
    }
    else
    {
        const std::string dir{
            boost::filesystem::initial_path<boost::filesystem::path>().string()};

        printf("%s.\r\n", dir.c_str());
    }

    return ret;
}

void LogHostServer::checkFileExpiredThread()
{
    XTime xt;
    const std::string temp{fileDir};
#ifdef _WINDOWS
    const std::size_t pos{temp.find_last_of('\\')};
#else
    const std::size_t pos{temp.find_last_of('/')};
#endif//_WINDOWS
    const std::string dir{temp.substr(0, pos + 1) + "log"};

    while(!stopped)
    {
        if (boost::filesystem::exists(dir))
        {
            boost::filesystem::directory_iterator item_begin(dir);
            boost::filesystem::directory_iterator item_end;
            for (; item_begin != item_end; item_begin++)
            {
                // if (boost::filesystem::is_directory(*item_begin))
                // {
                //     std::cout << item_begin->path() << "\t[dir]" << std::endl;
                // }
                // else
                // {
                //     std::cout << item_begin->path() << std::endl;
                // }
            }
        }

        xt.sleep(1000);
    }
}
