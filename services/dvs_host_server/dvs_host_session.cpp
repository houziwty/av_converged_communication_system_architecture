#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/time/async_timer.h"
using namespace framework::utils::time;
#include "dvs_host_server.h"
#include "dvs_host_session.h"

DvsHostSession::DvsHostSession(
    DvsHostServer& host,
    const std::string sid) 
    : TcpSession(), dvsHostServer{host}
    , sessionId{sid}, totalSendBytes{0}, totalRecvBytes{0}
{}

DvsHostSession::~DvsHostSession()
{}

int DvsHostSession::createNew(
    SessionPtr ptr, 
    const unsigned int bytes/* = 1048576*/)
{
    int ret{TcpSession::createNew(ptr, bytes)};

    if (Error_Code_Success == ret)
    {
        //设置接收超时定时器
        AsyncTimerPtr ptr{boost::make_shared<AsyncTimer>()};

        if(ptr)
        {
            ptr->setTimer(
                sessionPtr->sock(), 
                [&](const int e)
                {
                     //不用处理错误码
                    if(!totalRecvBytes)
                    {
                        dvsHostServer.removeExpiredSession(sessionId);
                    }
                });
        }
    }

    return ret;
}

void DvsHostSession::fetchSentDataEventNotification(const int e/* = 0*/, const int bytes/* = 0*/)
{
    totalSendBytes += bytes;
}

void DvsHostSession::fetchReceivedDataEventNotification(
    const int e/* = 0*/, 
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    totalRecvBytes += bytes;
}