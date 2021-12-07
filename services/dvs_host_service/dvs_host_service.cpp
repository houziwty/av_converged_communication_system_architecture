#include "boost/make_shared.hpp"
#include "liblog/log.h"
using namespace module::file::log;
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "avcap/sdk/hk_dvs_capture.h"
using namespace framework::media::av;
#include "error_code.h"
#include "dvs_host_session.h"
using DvsHostSessionPtr = boost::shared_ptr<DvsHostSession>;
#include "dvs_host_service.h"

DvsHostService::DvsHostService(FileLog& log, const unsigned short port/* = 10000*/) 
    : WorkerDeal()//, AsyncTcpServer(), xmsPort{port}, fileLog{log}
{}

DvsHostService::~DvsHostService()
{}

int DvsHostService::start(
    const std::string appid, 
    const std::string xmqid, 
    const std::string ip,
    const unsigned short port)
{
    int ret{WorkerDeal::start(appid, xmqid, ip, port)};

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Start XMQ service successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Start XMQ service failed, result = [ %d ] app_name = [ %s ] xmq_name ip = [ %s ] port = [ %d ].", 
            ret, appid.c_str(), xmqid.c_str(), ip.c_str(), port);
        return ret;
    }

    ret = AsyncTcpServer::createNew(xmsPort);

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Start XMS service successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Start XMS service failed, result = [ %d ] port = [ %d ].", 
            ret, xmsPort);
        return ret;
    }

    AVCapturePtr ptr{boost::make_shared<HKSdkCapture>()};
    if (ptr)
    {
        ret = ptr->createNew();

        if(Error_Code_Success == ret)
        {
            avcapturePtr.swap(ptr);
            fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Create Hikvision SDK capture service successed.");
        }
        else
        {
            fileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Create Hikvision SDK capture service failed, result = [ %d ].", ret);
        }
    }
    else
    {
        ret = Error_Code_Bad_New_Object;
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Create Hikvision SDK capture instance failed.");
    }
    
    return ret;
}

int DvsHostService::stop()
{
    AsyncTcpServer::destroy();
    WorkerDeal::stop();
    sessions.clear();
    if (avcapturePtr)
    {
        avcapturePtr->destroy();
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
