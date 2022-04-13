#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "zmq.h"
#include "map/unordered_map.h"
#include "error_code.h"
#include "core/ctx.h"
#include "module/data_dispatcher.h"
#include "module/worker_task.h"
#include "module/data_task.h"
#include "module/data_pub.h"
#include "module/data_sub.h"
#include "libxmq.h"
using namespace module::network::xmq;

using XMQNodePtr = boost::shared_ptr<XMQNode>;
static UnorderedMap<const uint32_t, XMQNodePtr> nodes;
static void* ctx{nullptr};

Libxmq::Libxmq()
{
	if (!ctx)
	{
		ctx = Ctx().createNew();
	}
}

Libxmq::~Libxmq()
{
	nodes.clear();
	Ctx().destroy(ctx);
}

int Libxmq::addNode(const XMQNodeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		XMQNodePtr node;

		if (XMQModuleType::XMQ_MODULE_TYPE_DISPATCHER == conf.type)
		{
			node = boost::make_shared<DataDispatcher>();
		}
		else if (XMQModuleType::XMQ_MODULE_TYPE_WORKER == conf.type)
		{
			node = boost::make_shared<WorkerTask>(
				boost::bind(&Libxmq::afterPolledXMQDataNotification, this, _1, _2, _3, _4));
		}
		else if (XMQModuleType::XMQ_MODULE_TYPE_TASK == conf.type)
		{
			node = boost::make_shared<DataTask>(
				boost::bind(&Libxmq::afterPolledXMQDataNotification, this, _1, _2, _3, _4), 
				boost::bind(&Libxmq::afterFetchOnlineStatusNotification, this, _1), 
				boost::bind(&Libxmq::afterFetchServiceCapabilitiesNotification, this, _1, _2));
		}
		else if (XMQModuleType::XMQ_MODULE_TYPE_PUB == conf.type)
		{
			node = boost::make_shared<DataPub>();
		}
		else if (XMQModuleType::XMQ_MODULE_TYPE_SUB == conf.type)
		{
			node = boost::make_shared<DataSub>(
				boost::bind(&Libxmq::afterPolledXMQDataNotification, this, _1, _2, _3, _4));
		}

		if (node && Error_Code_Success == node->run(conf, ctx))
		{
			nodes.add(conf.id, node);
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}

	return ret;
}

int Libxmq::removeNode(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		XMQNodePtr node{nodes.at(id)};
		ret = (node ? node->stop() : Error_Code_Object_Not_Exist);

		if (node)
		{
			nodes.remove(id);
		}
	}

	return ret;
}

int Libxmq::send(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* name/* = nullptr*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		XMQNodePtr node{ nodes.at(id)};
		ret = (node ? node->send(data, bytes, name) : Error_Code_Object_Not_Exist);
	}

	return ret;
}

void Libxmq::afterPolledXMQDataNotification(
    const uint32_t id/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
    const char* name/* = 0*/)
{}

void Libxmq::afterFetchOnlineStatusNotification(const bool online/* = false*/)
{}

void Libxmq::afterFetchServiceCapabilitiesNotification(
    const char** infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{}
