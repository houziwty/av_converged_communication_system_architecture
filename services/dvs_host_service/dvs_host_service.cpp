#include "boost/make_shared.hpp"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "avcap/sdk/hk_dvs_capture.h"
using namespace framework::media::av;
#include "error_code.h"
#include "dvs_host_session.h"
using DvsHostSessionPtr = boost::shared_ptr<DvsHostSession>;
#include "dvs_host_service.h"

DvsHostService::DvsHostService(const unsigned short port/* = 10000*/) 
    : WorkerDeal(), AsyncTcpServer(), xmsPort{port}
{}

DvsHostService::~DvsHostService()
{}

int DvsHostService::start(
    const std::string uid, 
    const std::string ip,
    const unsigned short port)
{
    int ret{WorkerDeal::start(uid, ip, port)};

    if (Error_Code_Success == ret)
    {
        WorkerDeal::keepalive(uid);
        AsyncTcpServer::createNew(xmsPort);
        AVCapturePtr ptr{boost::make_shared<HKSdkCapture>()};
        if (ptr && ptr->createNew())
        {
            capturePtr.swap(ptr);
        }
    }
    
    return ret;
}

int DvsHostService::stop()
{
    AsyncTcpServer::destroy();
    WorkerDeal::stop();
    sessions.clear();
    if (capturePtr)
    {
        capturePtr->destroy();
    }
    
    return Error_Code_Success;
}

void DvsHostService::removeExpiredSession(const std::string sid)
{
    sessions.remove(sid);
}

void DvsHostService::afterWorkerPolledDataHandler(const std::string data)
{
    if (!data.empty())
    {
    }
}

void DvsHostService::afterFetchAsyncAcceptEventNotification(boost::asio::ip::tcp::socket& s)
{
    const std::string sid{Uuid().createNew()};

    if(!sid.empty())
    {
        DvsHostSessionPtr ptr{boost::make_shared<DvsHostSession>(s, *this, sid)};

        if(ptr && 
            Error_Code_Success == ptr->createNew() &&
            Error_Code_Success == ptr->receive())
        {
            sessions.add(sid, ptr);
        }
    }
}
