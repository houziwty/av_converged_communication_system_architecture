#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "utils/map/unordered_map.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "service_discover.h"
#include "service_vendor.h"
#include "data_pub.h"
#include "data_sub.h"
#include "xmq_node.h"
using namespace module::network::xmq;

using XMQRolePtr = boost::shared_ptr<XMQRole>;
static UnorderedMap<const int, XMQRolePtr> roles;
static ctx_t ctx{nullptr};

XMQNode::XMQNode()
{
	ctx = Ctx().createNew();
}

XMQNode::~XMQNode()
{
	Ctx().destroy(ctx);
}

int XMQNode::addConf(const XMQModeConf& conf)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (
			XMQModeType::XMQ_MODE_TYPE_NONE < conf.type && conf.name && conf.ip && 0 < conf.port && 0 < conf.id ? 
			Error_Code_Success : 
			Error_Code_Invalid_Param);

		if (Error_Code_Success == ret)
		{
			XMQRolePtr role;

			if (XMQModeType::XMQ_MODE_TYPE_ROUTER == conf.type)
			{
				role = boost::make_shared<ServiceDiscover>(
					conf, 
					boost::bind(&XMQNode::afterPolledDataNotification, this, _1, _2, _3, _4));
			}
			else if (XMQModeType::XMQ_MODE_TYPE_DEALER == conf.type)
			{
				role = boost::make_shared<ServiceVendor>(
					conf,
					boost::bind(&XMQNode::afterPolledDataNotification, this, _1, _2, _3, _4), 
					boost::bind(&XMQNode::afterFetchOnlineStatusNotification, this, _1), 
					boost::bind(&XMQNode::afterFetchServiceCapabilitiesNotification, this, _1, _2));
			}
			else if (XMQModeType::XMQ_MODE_TYPE_PUB == conf.type)
			{
				role = boost::make_shared<DataPub>(conf);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_SUB == conf.type)
			{
				role = boost::make_shared<DataSub>(
					conf, 
					boost::bind(&XMQNode::afterPolledDataNotification, this, _1, _2, _3, _4));
			}

			if (role)
			{
				roles.add(conf.id, role);
			}
			else
			{
				ret = Error_Code_Bad_New_Object;
			}
		}
	}

	return ret;
}

int XMQNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{ctx && 0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		XMQRolePtr role{roles.at(id)};

		if (role)
		{
			ret = role->stop();
			roles.remove(id);
		}
	}

	return ret;
}

int XMQNode::run()
{
	int ret{ctx ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		std::vector<XMQRolePtr> items{roles.values()};
		for (int i = 0; i != items.size(); ++i)
		{
			if (items[i])
			{
				items[i]->run(ctx);
			}
		}
	}

	return ret;
}

int XMQNode::stop()
{
	int ret{ctx ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		std::vector<XMQRolePtr> items{roles.values()};
		for (int i = 0; i != items.size(); ++i)
		{
			if (items[i])
			{
				items[i]->stop();
			}
		}
	}

	return ret;
}

int XMQNode::send(
	const uint32_t id/* = 0*/, 
	const char* name/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{ctx ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		XMQRolePtr role{roles.at(id)};

		if (role)
		{
			ret = role->send(name, data, bytes);
		}
	}

	return ret;
}
