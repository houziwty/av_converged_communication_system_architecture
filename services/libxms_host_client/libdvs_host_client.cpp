#include "boost/make_shared.hpp"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "error_code.h"
#include "dvs_host_client.h"
using DvsHostClientPtr = boost::shared_ptr<DvsHostClient>;
#include "libdvs_host_client.h"

DvsHostClientPtr dvsHostClientPtr;

DvsHostClient::DvsHostClient()
{}

DvsHostClient::~DvsHostClient()
{}

int DvsHostClient::connectXMQService(
    const std::string name, 
    const std::string ip, 
    const unsigned short port/* = 0*/)
{
    if (dvsHostClientPtr)
    {
        return Error_Code_Object_Existed;
    }

    int ret{!name.empty() && !ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string appid{Uuid().createNew()};
        DvsHostClientPtr ptr{boost::make_shared<DvsHostClient>()};
        
        if (ptr && Error_Code_Success == ptr->start(appid, name, ip, port))
        {
            dvsHostClientPtr.swap(ptr);
        }
    }

    return dvsHostClientPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int DvsHostClient::disconnectXMQService()
{
    int ret{dvsHostClientPtr ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        dvsHostClientPtr->stop();
        dvsHostClientPtr.reset();
    }
    
    return ret;
}
