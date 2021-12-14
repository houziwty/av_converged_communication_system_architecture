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
    : WorkerModel(), libXmqHostClient{client}, registerResponseTimetamp{0}, thread{nullptr}
{}

XmqHostClient::~XmqHostClient()
{}

int XmqHostClient::start(
    const std::string name, 
    const std::string ip, 
    const unsigned short port/* = 0*/)
{
    int ret{WorkerModel::start(name, ip, port)};

    if (Error_Code_Success == ret)
    {
        thread = ThreadPool().get_mutable_instance().createNew(
            boost::bind(&XmqHostClient::processTaskOnTimerThread, this));
    }
    
    return ret;
}

int XmqHostClient::stop()
{
    int ret{WorkerModel::stop()};

    if (Error_Code_Success == ret && stopped)
    {
        Thread().join(thread);
        ThreadPool().get_mutable_instance().destroy(thread);
    }
    
    return ret;
}

void XmqHostClient::afterWorkerPolledDataHandler(const std::string data)
{
    Url url;

    if (Error_Code_Success == url.parse(data))
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
    std::vector<std::string> services;
    const std::vector<ParamItem> items{url.getParameters()};

    for(int i = 0; i != items.size(); ++i)
    {
        if(!items[i].key.compare("name"))
        {
            services.push_back(items[i].value);
        }
    }

    libXmqHostClient.fetchXmqHostServiceCapabilitiesNotification(services);
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
            WorkerModel::send((boost::format("query://%s") % xmqHostServiceName).str());
            lastTickcount = currentTickcount;
        }

		xt.sleep(1000);
	}
}
