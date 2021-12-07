//#include "error_code.h"
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "dvs_host_client.h"

DvsHostClient::DvsHostClient() : WorkerDeal()
{}

DvsHostClient::~DvsHostClient()
{}

int DvsHostClient::start(
    const std::string appid, 
    const std::string xmqid, 
    const std::string ip,
    const unsigned short port)
{
    int ret{WorkerDeal::start(appid, xmqid, ip, port)};

    tp.createNew(boost::bind(&DvsHostClient::stop, this));
    return ret;
}

int DvsHostClient::stop()
{
    return WorkerDeal::stop();
}

void DvsHostClient::afterWorkerPolledDataHandler(const std::string data)
{
    if (!data.empty())
    {
    }
}
