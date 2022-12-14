#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "hikvision/hikvision_node.h"
#include "dahua/dahua_node.h"
#include "uniview/uniview_node.h"
#include "map/unordered_map.h"
#include "libdvs.h"
using namespace module::device::dvs;

using DVSNodePtr = boost::shared_ptr<DVSNode>;
static UnorderedMap<const uint32_t, DVSNodePtr> dvss;

Libdvs::Libdvs()
{}

Libdvs::~Libdvs()
{
	dvss.clear();
}

int Libdvs::addConf(const DVSModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};
	DVSNodePtr dvs;

	if (Error_Code_Success == ret)
	{
		if (DVSFactoryType::DVS_FACTORY_TYPE_HK == conf.factory)
		{
			dvs = boost::make_shared<HikvisionNode>(
				boost::bind(&Libdvs::afterPolledDVSDataNotification, this, _1, _2, _3, _4, _5), 
				boost::bind(&Libdvs::afterPolledDVSExceptionNotification, this, _1, _2, _3, _4));
		}
		else if (DVSFactoryType::DVS_FACTORY_TYPE_DH == conf.factory)
		{
			dvs = boost::make_shared<DahuaNode>(
				boost::bind(&Libdvs::afterPolledDVSDataNotification, this, _1, _2, _3, _4, _5), 
				boost::bind(&Libdvs::afterPolledDVSExceptionNotification, this, _1, _2, _3, _4));
		}
		else if (DVSFactoryType::DVS_FACTORY_TYPE_UNIVIEW == conf.factory)
		{
			dvs = boost::make_shared<UniviewNode>(
				boost::bind(&Libdvs::afterPolledDVSDataNotification, this, _1, _2, _3, _4, _5), 
				boost::bind(&Libdvs::afterPolledDVSExceptionNotification, this, _1, _2, _3, _4));
		}
		else
		{
			ret = Error_Code_Invalid_Param;
		}

		if (dvs)
		{
			ret = dvs->run(conf);

			if (Error_Code_Success == ret)
			{
				dvss.add(conf.id, dvs);
			}
		}
	}

	return ret;
}

int Libdvs::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DVSNodePtr dvs{dvss.at(id)};

		if (dvs)
		{
			dvs->stop();
			dvss.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int Libdvs::queryConf(
	const uint32_t id/* = 0*/, 
	const DVSConfParamType type/* = DVSConfParamType::DVS_CONF_PARAM_TYPE_NONE*/, 
	void* param/* = nullptr*/)
{
	int ret{0 < id && param ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DVSNodePtr node{dvss.at(id)};

		if(node)
		{
			if (DVSConfParamType::DVS_CONF_PARAM_TYPE_CHANNEL_NUMBER == type)
			{
				uint32_t* channels{reinterpret_cast<uint32_t*>(param)};
				*channels = node->channums();
			}
			else
			{
				ret = Error_Code_Method_Not_Support;
			}
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
