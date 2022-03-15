#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "dvs.h"
using DvsPtr = boost::shared_ptr<Dvs>;
#include "error_code.h"
#include "map/unordered_map.h"
#include "libvideo_encode_image.h"

UnorderedMap<const int, DvsPtr> dvss;
SharedMutex mtx;
static int deviceId{0};

static const int getDeviceId(void)
{
    //WriteLock wl{mtx};
    mtx.lock();
    int number{++deviceId};
    mtx.unlock();
    return number;
}

int CLT_LIB_VideoEncodeImage_Start()
{
    return Error_Code_Success;
}

int CLT_LIB_VideoEncodeImage_Stop()
{
    return Error_Code_Success;
}

int CLT_LIB_VideoEncodeImage_AddDevice(
    const std::string username, 
    const std::string passwd, 
    const std::string ip, 
    const unsigned short port)
{
    int ret{
        !username.empty() && !passwd.empty() && !ip.empty() && 0 < port ? 
        Error_Code_Success : 
        Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        const int did{getDeviceId()};
        DvsPtr ptr{boost::make_shared<Dvs>(did)};

        if(0 < did && ptr && -1 < (ret = ptr->login(ip, port, username, passwd)))
        {
            dvss.replace(did, ptr);
            ret = did;
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }

    return ret;
}

int CLT_LIB_VideoEncodeImage_RemoveDevice(const int did/* = -1*/)
{
    int ret{0 < did ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        DvsPtr ptr{dvss.at(did)};
        
        if(ptr)
        {
            dvss.remove(did);
        }
        else
        {
            ret = Error_Code_Object_Not_Exist;
        }
    }

    return ret;
}

int CLT_LIB_VideoEncodeImage_GetFrame(const int did, OutputFrameData*& ofd)
{
    int ret{0 < did ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        DvsPtr ptr{dvss.at(did)};
        
        if(ptr)
        {
            ptr->getOneFrameData(ofd);
        }
        else
        {
            ret = Error_Code_Object_Not_Exist;
        }
    }

    return ret;
}
