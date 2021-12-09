#include "boost/make_shared.hpp"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "error_code.h"
#include "xmq_host_client.h"
using XmqHostClientPtr = boost::shared_ptr<XmqHostClient>;
#include "libxmq_host_client.h"

XmqHostClientPtr xmqHostClientPtr;

LibXmqHostClient::LibXmqHostClient()
{}

LibXmqHostClient::~LibXmqHostClient()
{
    disconnect();
}

int LibXmqHostClient::connect(
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
        const std::string appid{Uuid().createNew()};
        XmqHostClientPtr ptr{boost::make_shared<XmqHostClient>()};
        
        if (ptr && Error_Code_Success == ptr->start(appid, name, ip, port))
        {
            xmqHostClientPtr.swap(ptr);
        }
    }

    return xmqHostClientPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int LibXmqHostClient::disconnect()
{
    int ret{xmqHostClientPtr ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        xmqHostClientPtr->stop();
        xmqHostClientPtr.reset();
    }
    
    return ret;
}
