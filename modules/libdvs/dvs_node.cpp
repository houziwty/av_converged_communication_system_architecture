#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "hikvision/hikvision_device.h"
#include "utils/map/unordered_map.h"
#include "dvs_node.h"
using namespace module::device::dvs;

using DevicePtr = boost::shared_ptr<Device>;
static UnorderedMap<const uint32_t, DevicePtr> devices;

DVSNode::DVSNode()
{}

DVSNode::~DVSNode()
{
	devices.clear();
}

int DVSNode::addConf(const DVSModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DevicePtr device{devices.at(conf.id)};

		if(!device)
		{
			if (DVSFactoryType::DVS_FACTORY_TYPE_HK == conf.factory)
			{
				device = boost::make_shared<HikvisionDevice>(
					conf, 
					boost::bind(&DVSNode::afterPolledRealplayDataNotification, this, _1, _2, _3, _4, _5));
			}
			else
			{
				ret = Error_Code_Operate_Not_Support;
			}
		}
		else
		{
			ret = Error_Code_Object_Existed;
		}

		if (Error_Code_Success == ret && device)
		{
			devices.add(conf.id, device);
		}
	}

	return ret;
}

int DVSNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DevicePtr device{devices.at(id)};

		if (device)
		{
			device->stop();
			devices.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int DVSNode::queryConf(std::vector<DVSModeConf>& confs)
{
	confs.clear();
	const std::vector<DevicePtr> dvss{devices.values()};

	for (int i = 0; i != dvss.size(); ++i)
	{
		confs.push_back(dvss[i]->getConf());
	}

	return Error_Code_Success;
}

const DVSModeConf DVSNode::queryConf(const uint32_t id/* = 0*/)
{
	DVSModeConf conf{0};
	DevicePtr device{devices.at(id)};

	if (device)
	{
		conf = device->getConf();
	}
	
	return conf;
}

int DVSNode::run(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DevicePtr device{devices.at(id)};
		ret = (device ? device->run() : Error_Code_Object_Not_Exist);
	}

	return ret;
}

int DVSNode::stop(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DevicePtr device{devices.at(id)};
		ret = (device ? device->stop() : Error_Code_Object_Not_Exist);
	}

	return ret;
}