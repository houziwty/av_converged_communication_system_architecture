#include "boost/make_shared.hpp"
#include "error_code.h"
#include "xmq_host_client.h"
using XmqHostClientPtr = boost::shared_ptr<XmqHostClient>;
#include "libxmq_host_client.h"

XmqHostClientPtr xmqHostClientPtr;

LibXmqHostClient::LibXmqHostClient()
{}

LibXmqHostClient::~LibXmqHostClient()
{
    unregisterXmqHostClient();
}

int LibXmqHostClient::registerXmqHostClient(
    const std::string name, 
    const std::string ip, 
    const unsigned short port/* = 0*/)
{
    if (xmqHostClientPtr)
    {
        return Error_Code_Object_Existed;
    }

    int ret{!name.empty() && !ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        XmqHostClientPtr ptr{boost::make_shared<XmqHostClient>(*this)};
        
        if (ptr && Error_Code_Success == ptr->start(name, ip, port))
        {
            xmqHostClientPtr.swap(ptr);
        }
    }

    return xmqHostClientPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int LibXmqHostClient::unregisterXmqHostClient()
{
    int ret{xmqHostClientPtr ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        xmqHostClientPtr->stop();
        xmqHostClientPtr.reset();
    }
    
    return ret;
}

int LibXmqHostClient::send(const std::string data)
{
    return xmqHostClientPtr ? xmqHostClientPtr->send(data) : Error_Code_Operate_Failure;
}
