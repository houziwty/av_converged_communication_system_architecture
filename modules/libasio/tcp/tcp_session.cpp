#include "error_code.h"
#include "tcp_session.h"
using namespace module::network::asio;

TcpSession::TcpSession()
{}

TcpSession::~TcpSession()
{}

int TcpSession::createNew(SessionPtr ptr, const unsigned int bytes/* = 1048576*/)
{
    if (!sessionPtr)
    {
        return Error_Code_Operate_Failure;
    }
    
    int ret{0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        sessionPtr.swap(ptr);
        ret = sessionPtr->createNew(
            [&](const int e, const int bytes)
            {
                fetchSentDataEventNotification(e, bytes);
            },
            [&](const int e, const void* data, const int bytes)
            {
                fetchReceivedDataEventNotification(e, data, bytes);

                //持续接收
                if (!e)
                {
                    receive();
                }
            },
            bytes);
    }
    
    return ret;
}

int TcpSession::destroy()
{
    return sessionPtr ? sessionPtr->destroy() : Error_Code_Operate_Failure;
}

int TcpSession::send(
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    if (!sessionPtr)
    {
        return Error_Code_Operate_Failure;
    }

    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = sessionPtr->send(data, bytes);
    }
    
    return ret;
}

int TcpSession::receive()
{
    return sessionPtr ? sessionPtr->receive() : Error_Code_Operate_Failure;
}
