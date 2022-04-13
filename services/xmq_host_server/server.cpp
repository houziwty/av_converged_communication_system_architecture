#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "hardware/cpu.h"
using namespace framework::utils::hardware;
#include "time/xtime.h"
using namespace framework::utils::time;

#include "server.h"

Server::Server(FileLog& flog)
    : log{flog}, stopped{false}, offlineCheckThread{nullptr}, consumers{nullptr}
{}

Server::~Server()
{
    stop();
}

int Server::run(const uint16_t port/* = 0*/)
{
    int ret{dispatcher.run(port)};

    if (Error_Code_Success == ret)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Run xmq server listen port [ %u ] successfully.", port);
        
        for (int i = 0; i != Cpu().getCount(); ++i)
        {
            boost::shared_ptr<Consumer> ptr{
                boost::make_shared<Consumer>(*this)};

            if (ptr)
            {
                ret = ptr->run(0xFF00 + i);

                if (Error_Code_Success == ret)
                {
                    consumers.push_back(ptr);
                    log.write(
                        SeverityLevel::SEVERITY_LEVEL_INFO,
                        "Run data consumer ID [ %d ] successfully.", 0xFF00 + i);
                }
                else
                {
                    log.write(
                        SeverityLevel::SEVERITY_LEVEL_ERROR,
                        "Run data consumer ID [ %d ] failed, result = [ %d ].", 0xFF00 + i, ret);
                }
            }
        }
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR,
            "Run xmq server listen port [ %u ] failed.", port);
    }

    offlineCheckThread = ThreadPool().get_mutable_instance().createNew(
        boost::bind(&Server::servicesOfflineCheckThread, this));
    return Error_Code_Success;
}

int Server::stop()
{
    stopped = true;
    ThreadPool().get_mutable_instance().destroy(offlineCheckThread);
    return Error_Code_Success;
}

int Server::processRegisterRequest(
    const char* name/* = nullptr*/, 
    const uint64_t timestamp/* = 0*/)
{
    int ret{name && 0 < timestamp ? Error_Code_Success : Error_Code_Bad_RequestUrl};

    if (Error_Code_Success == ret)
    {
        onlineServices.replace(name, timestamp);
    }

    return ret;
}

const std::string Server::processQueryRequest()
{
    std::string out;
    const std::vector<std::string> names{onlineServices.keies()};

    for(int i = 0; i != names.size(); ++i)
    {
        out += (out.empty() ? "name=" : "&name=");
        out += names[i];
    }
    
    return out;
}

void Server::servicesOfflineCheckThread()
{
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
        "Start offline status of services checking thread.");

	while (!stopped)
	{
        XTime xt;
        const uint64_t startTickCount{xt.tickcount()};
        const std::vector<std::string> keies{onlineServices.keies()};

        for (int i = 0; i != keies.size(); ++i)
        {
            const uint64_t tick{onlineServices.at(keies[i])};
            const uint64_t diff{startTickCount > tick ? startTickCount - tick : tick - startTickCount};

            if (diff > 90000)
            {
                onlineServices.remove(keies[i]);
				log.write(
					SeverityLevel::SEVERITY_LEVEL_WARNING,
					"Remove service name [ %s ] while time out, diff [ %lld ].",
                    keies[i].c_str(), diff);
            }
        }
        
		xt.sleep(1000);
	}

    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
        "Stop offline status of services checking thread.");
}

void Server::catchExceptionOfParsingUrl(
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/, 
    const int ret/* = 0*/)
{
    const std::string text{ (const char*)data, bytes };
    log.write(
        SeverityLevel::SEVERITY_LEVEL_WARNING,
        "Parse request message [ %s ] from [ %s ] failed, result [ %d ].",
        text.c_str(), name, ret);
}
