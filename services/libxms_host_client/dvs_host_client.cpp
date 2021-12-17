#include "error_code.h"
#include "dvs_host_client.h"

DvsHostClient::DvsHostClient() : WorkerModel()
{}

DvsHostClient::~DvsHostClient()
{}

int DvsHostClient::start(
    const std::string appid, 
    const std::string xmqid, 
    const std::string ip,
    const unsigned short port)
{
    int ret{WorkerModel::start(appid, xmqid, ip, port)};
    
    if(Error_Code_Success == ret)
    {
        ret = service.start();
    }

    return ret;
}

int DvsHostClient::stop()
{
    WorkerModel::stop();
    service.stop();
    return Error_Code_Success;
}

void DvsHostClient::afterWorkerPolledDataHandler(const std::string data)
{
    if (!data.empty())
    {
    }
}
