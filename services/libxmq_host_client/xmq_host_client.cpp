#include "utils/url/url.h"
using namespace framework::utils::url;
#include "xmq_host_client.h"

XmqHostClient::XmqHostClient() : WorkerModel()
{}

XmqHostClient::~XmqHostClient()
{}

int XmqHostClient::queryOnlineService()
{
    //Register and heartbeat
    Url url;
    url.setProtocol("register");
    url.setHost(xmqid);
    url.addParameter("tick", (boost::format("%ld") % currentTickCount).str());
    url.addParameter("sequence", (boost::format("%d") % sequence++).str());
    const std::string data{url.encode()};

    if(!data.empty())
    {
        send(data);
    }
}

void DvsHostClient::afterWorkerPolledDataHandler(const std::string data)
{
    if (!data.empty())
    {
    }
}
