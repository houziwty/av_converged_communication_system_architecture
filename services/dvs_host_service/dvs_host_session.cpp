#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/time/async_timer.h"
using namespace framework::utils::time;
#include "dvs_host_service.h"
#include "dvs_host_session.h"

DvsHostSession::DvsHostSession(
    boost::asio::ip::tcp::socket& s, 
    DvsHostService& dhs,
    const std::string sid) 
    : AsyncTcpSession(s), dvsHostService{dhs}, sessionId{sid}, totalSendBytes{0}, totalRecvBytes{0}
{}

DvsHostSession::~DvsHostSession()
{}

int DvsHostSession::createNew(const int bytes/* = 1048576*/)
{
    int ret{AsyncTcpSession::createNew(bytes)};

    if (Error_Code_Success == ret)
    {
        //设置接收超时定时器
        AsyncTimerPtr ptr{boost::make_shared<AsyncTimer>()};

        if(ptr)
        {
            ptr->setTimer(so, boost::bind(&DvsHostSession::afterFetchExpiredEventNotification, this, _1));
        }
    }

    return ret;
}

void DvsHostSession::afterFetchExpiredEventNotification(const int e/* = 0*/)
{
    //不用处理错误码

    if(!totalRecvBytes)
    {
        dvsHostService.removeExpiredSession(sessionId);
    }
}

void DvsHostSession::afterFetchSendEventNotification(const int e/* = 0*/, const int bytes/* = 0*/)
{
    totalSendBytes += bytes;
}

void DvsHostSession::asyncFetchReceiveEventNotification(const int e/* = 0*/, const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
    totalRecvBytes += bytes;
}
