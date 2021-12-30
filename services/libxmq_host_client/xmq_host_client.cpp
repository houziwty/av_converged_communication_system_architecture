#include "boost/format.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "libxmq_host_client.h"
#include "xmq_host_client.h"

XmqHostClient::XmqHostClient(LibXmqHostClient& client) 
    : WorkerMode(), libXmqHostClient{client}, registerResponseTimetamp{0}, thread{nullptr}
{}

XmqHostClient::~XmqHostClient()
{}

int XmqHostClient::start(
    const void* name/* = nullptr*/, 
	const int bytes/* = 0*/,  
	const char* ip/* = nullptr*/, 
	const unsigned short port/* = 0*/)
{
    int ret{WorkerMode::start(name, bytes, ip, port)};

    if (Error_Code_Success == ret)
    {
        thread = ThreadPool().get_mutable_instance().createNew(
            boost::bind(&XmqHostClient::processTaskOnTimerThread, this));
    }
    
    return ret;
}

int XmqHostClient::stop()
{
    int ret{WorkerMode::stop()};

    if (Error_Code_Success == ret && stopped)
    {
        Thread().join(thread);
        ThreadPool().get_mutable_instance().destroy(thread);
    }
    
    return ret;
}

void XmqHostClient::afterWorkerPolledDataHandler(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
    if (data && 0 < bytes)
    {
        const std::string msg{
            (const char*)data, 
            static_cast<unsigned int>(bytes)};
        Url url;

        if (Error_Code_Success == url.parse(msg))
        {
            const std::string protocol{url.getProtocol()};
            if (!protocol.compare("register"))
            {
                processRegisterResponseMessage(url);
            }
            else if (!protocol.compare("query"))
            {
                processQueryResponseMessage(url);
            }
            else
            {
                libXmqHostClient.fetchXmqHostClientReceivedDataNotification(data);
            }
        }
    }
}

void XmqHostClient::processRegisterResponseMessage(Url& url)
{
    xmqHostServiceName = url.getHost();
    const std::vector<ParamItem> items{url.getParameters()};

    for(int i = 0; i != items.size(); ++i)
    {
        if(!items[i].key.compare("timestamp"))
        {
            registerResponseTimetamp = std::stoull(items[i].value);
            break;
        }
    }
}

void XmqHostClient::processQueryResponseMessage(Url& url)
{
    ServiceInfo* infos{nullptr};
    const std::vector<ParamItem> items{url.getParameters()};
    const std::size_t number{items.size()};

    if (0 < number)
    {
        infos = new(std::nothrow) ServiceInfo[number];
    }

    for(int i = 0; i != number; ++i)
    {
        if(!items[i].key.compare("name"))
        {
            infos[i].name = items[i].value.c_str();
        }
    }

    libXmqHostClient.fetchXmqHostServiceCapabilitiesNotification(infos, static_cast<int>(number));
    boost::checked_array_delete(infos);
}

void XmqHostClient::processTaskOnTimerThread()
{
    bool online{false};
    unsigned long long lastTickcount{0};
    XTime xt;

	while (!stopped)
	{
        const unsigned long long currentTickcount{xt.tickcount()};
        const unsigned long long diff{currentTickcount - registerResponseTimetamp};

        if (diff >= 90000 && online)
        {
            online = false;
            libXmqHostClient.fetchXmqHostClientOnlineStatusNotification(online);
        }
        else if(diff < 90000 && !online)
        {
            online = true;
            libXmqHostClient.fetchXmqHostClientOnlineStatusNotification(online);
        }

        if (30000 < currentTickcount - lastTickcount && !xmqHostServiceName.empty())
        {
            const std::string msg{(boost::format("query://%s") % xmqHostServiceName).str()};
            WorkerMode::send(msg.c_str(), msg.length());
            lastTickcount = currentTickcount;
        }

		xt.sleep(1000);
	}
}
