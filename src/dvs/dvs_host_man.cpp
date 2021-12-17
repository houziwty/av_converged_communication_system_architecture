#include "error_code.h"
#include "libdvshost/device.h"
#include "libdvshost/stream.h"
using namespace module::dvs;
#include "dvs/dvs_host_man.h"
using namespace framework::dvs;

DvsHostMan::DvsHostMan()
{}

DvsHostMan::~DvsHostMan()
{}

int DvsHostMan::addDevice(
    const std::string uuid, 
    const std::string username, 
    const std::string userpwd, 
    const std::string ip, 
    const unsigned short port/* = 0*/, 
    const FactoryType factory/* = FactoryType::FACTORY_TYPE_NONE*/)
{
    int ret{
		FactoryType::FACTORY_TYPE_NONE < factory && !uuid.empty() && 
        !username.empty() && !userpwd.empty() && !ip.empty() && 0 < port ? 
		Error_Code_Success : 
		Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
        DeviceInfo dvsinfo;
        dvsinfo.dvs = dvsHostCreator.createNewDevice(factory);
        if (dvsinfo.dvs && -1 < (ret = dvsinfo.dvs->login(username, userpwd, ip, port)))
        {
            dvsinfo.userID = ret;
            dvsinfo.cameras = dvsinfo.dvs->cameras();
            dvss.replace(uuid, dvsinfo);
        }
	}

	return -1 < ret ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int DvsHostMan::removeDevice(const std::string uuid)
{
    int ret{!uuid.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        DeviceInfo dvsinfo{dvss.at(uuid)};

        if (dvsinfo.dvs)
        {
            dvsinfo.dvs->logout();
            dvss.remove(uuid);
        }

        ret = (dvsinfo.dvs ? Error_Code_Success : Error_Code_Object_Not_Exist);
    }
    
    return ret;
}

const CameraPtrs DvsHostMan::queryCameraPtrs(const std::string uuid)
{
    CameraPtrs cameraPtrs;
    DeviceInfo dvsinfo{dvss.at(uuid)};

    if (dvsinfo.dvs)
    {
        cameraPtrs = dvsinfo.cameras;
    }

    return std::move(cameraPtrs);
}

int DvsHostMan::openStream(
    const std::string streamID, 
    const std::string dvsID, 
    const FactoryType type/* = FactoryType::FACTORY_TYPE_NONE*/, 
    const int userID/* = -1*/, 
    const int index/* = -1*/, 
    const unsigned int streamType/* = 0*/)
{
    DeviceInfo dvsinfo{dvss.at(dvsID)};
    int ret{dvsinfo.dvs ? Error_Code_Success : Error_Code_Object_Not_Exist};

    if(Error_Code_Success == ret)
    {
        StreamInfo streaminfo;
        streaminfo.stream = dvsHostCreator.createNewStream(type);

        if (streaminfo.stream && (ret = streaminfo.stream->open(userID, index, streamType)))
        {
            streaminfo.streamID = ret;
            streaminfo.userID = userID;
            streams.replace(streamID, streaminfo);
        }
    }

    return -1 < ret ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int DvsHostMan::closeStream(const std::string uuid)
{
    int ret{!uuid.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        StreamInfo streaminfo{streams.at(uuid)};

        if (streaminfo.stream)
        {
            streaminfo.stream->close(streaminfo.streamID);
            streams.remove(uuid);
        }

        ret = (streaminfo.stream ? Error_Code_Success : Error_Code_Object_Not_Exist);
    }
    
    return ret;
}
