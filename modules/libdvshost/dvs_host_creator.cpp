#include "boost/make_shared.hpp"
#include "hikvision/hikvision_stream.h"
#include "hikvision/hikvision_camera.h"
#include "hikvision/hikvision_device.h"
#include "error_code.h"
#include "dvs_host_creator.h"
using namespace module::dvs;

DvsHostCreator::DvsHostCreator()
{}

DvsHostCreator::~DvsHostCreator()
{}

DevicePtr DvsHostCreator::createNewDevice(
	const FactoryType type /* = FactoryType::FACTORY_TYPE_NONE */)
{
	DevicePtr devicePtr;

	if (FactoryType::FACTORY_TYPE_HK == type)
	{
		DevicePtr ptr{boost::make_shared<HikvisionDevice>()};

		if(ptr)
		{
			devicePtr.swap(ptr);
		}
	}

	return devicePtr;
}

int DvsHostCreator::destroyDevice(DevicePtr dvs)
{
	int ret{dvs ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		dvs.reset();
	}

	return ret;
}

const CameraPtrs DvsHostCreator::cameras(const DevicePtr dvs)
{
	return dvs ? std::move(dvs->cameras()) : std::move(CameraPtrs());
}

StreamPtr DvsHostCreator::createNewStream(
	const FactoryType factory/* = FactoryType::FACTORY_TYPE_NONE*/)
{
	StreamPtr streamPtr;

	if (FactoryType::FACTORY_TYPE_HK == factory)
	{
		StreamPtr ptr{boost::make_shared<HikvisionStream>()};

		if(ptr)
		{
			streamPtr.swap(ptr);
		}
	}

	return streamPtr;
}

int DvsHostCreator::destroyStream(StreamPtr stream)
{
	int ret{stream ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		stream.reset();
	}

	return ret;
}
