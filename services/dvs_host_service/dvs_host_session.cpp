#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "libasio/session.h"
using namespace module::network::asio;
#include "error_code.h"
#include "utils/time/async_timer.h"
using namespace framework::utils::time;
#include "dvs_host_service.h"
#include "dvs_host_session.h"

DvsHostSession::DvsHostSession(
    SessionPtr ptr, 
    DvsHostService& dhs,
    const std::string sid) 
    : TcpSession(ptr), dvsHostService{dhs}, sessionId{sid}, totalSendBytes{0}, totalRecvBytes{0}
{}

DvsHostSession::~DvsHostSession()
{}

int DvsHostSession::createNew(const unsigned int bytes/* = 1048576*/)
{
    int ret{TcpSession::createNew(bytes)};

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
                        dvsHostService.removeExpiredSession(sessionId);
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
