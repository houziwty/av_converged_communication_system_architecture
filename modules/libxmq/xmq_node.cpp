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
#include "xmq/dealer.h"
#include "xmq/pub.h"
#include "xmq/sub.h"
#include "xmq/msg.h"
#include "service_discover.h"
#include "xmq_node.h"
using namespace module::network::xmq;

static UnorderedMap<socket_t, const XMQModeConf> confs;
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
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Bad_Operate};

	if (Error_Code_Success == ret)
	{
		ret = (
			XMQModeType::XMQ_MODE_TYPE_NONE < conf.type && conf.name && conf.ip && 0 < conf.port && 0 < conf.id ? 
			Error_Code_Success : 
			Error_Code_Invalid_Param);

		if (Error_Code_Success == ret)
		{
			if (XMQModeType::XMQ_MODE_TYPE_ROUTER == conf.type)
			{
			}
			else if (XMQModeType::XMQ_MODE_TYPE_DEALER == conf.type)
			{
			}
			else if (XMQModeType::XMQ_MODE_TYPE_PUB == conf.type)
			{
			}
			else if (XMQModeType::XMQ_MODE_TYPE_SUB == conf.type)
			{
			}
			else
			{
				ret = Error_Code_Operate_Not_Support;
			}
		}
	}

	return ret;
}

int XMQNode::remove(const uint32_t id/* = 0*/)
{
	int ret{ctx && stopped && 0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		const XMQNodeConf& conf{confs.at(id)};

		if (0 < conf.id && conf.so)
		{
			if (XMQModeType::XMQ_MODE_TYPE_ROUTER == conf.type)
			{
				ret = Router().shutdow(conf.so);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_DEALER == conf.type)
			{
				ret = Dealer().shutdow(conf.so);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_PUB == conf.type)
			{
				ret = Pub().shutdow(conf.so);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_SUB == conf.type)
			{
				ret = Sub().shutdow(conf.so);
			}

			confs.remove(id);
		}
		
	}

	return ret;
}

int XMQNode::run()
{
	int ret{ctx ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		/* code */
	}
	
}

int XMQNode::send(
	const uint32_t id/* = 0*/, 
	const char* name/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{ctx && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			const XMQNodeConf& conf{confs.at(id)};

			if (XMQModeType::XMQ_MODE_TYPE_ROUTER == conf.type && name)
			{
				const std::string sid{name};
				msg.append(sid.c_str(), sid.length());
				msg.append("", 0);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_DEALER == conf.type)
			{
				msg.append("", 0);
			}
			else if (XMQModeType::XMQ_MODE_TYPE_PUB == conf.type)
			{
			}
			else
			{
				ret = Error_Code_Operate_Not_Support;
			}

			if (Error_Code_Success == ret)
			{
				msg.append(data, bytes);
				ret = msg.send(router);
			}
		}
	}

	return ret;
}

void XMQNode::pollDataThread()
{
	while(ctx && !stopped) 
	{
		const std::vector<const XMQNodeConf> nodeconfs{confs.value()};

		zmq_pollitem_t pollitems[]{ { router, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			ret = msg.recv(router);
			if (Error_Code_Success == ret)
			{
				//只读第一和第三段数据
 				const Message* header{ msg.msg() };
				const Message* body{ msg.msg(2) };
				
				if (handler)
				{
					handler(header->data, header->bytes, body->data, body->bytes);
				}
			}
		}
	}
}
