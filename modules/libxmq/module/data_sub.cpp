#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "core/sub.h"
#include "core/sock.h"
#include "data_sub.h"
using namespace module::network::xmq;

DataSub::DataSub(PolledDataCallback callback) 
	: XMQNode(callback), sso{nullptr}, id{0}
{}

DataSub::~DataSub()
{}

int DataSub::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		sso = Sub().connect(ctx, conf.ip, conf.port, conf.opt.reconivl);

		if(sso)
		{
			id = conf.id;
			ret = XMQNode::run(conf, ctx);
		}
	}

	return ret;
}

int DataSub::stop()
{
	int ret{XMQNode::stop()};

	if (Error_Code_Success == ret)
	{
		Sub().shutdown(sso);
	}
	
	return ret;
}

int DataSub::send(
	const void* /*data = nullptr*/, 
	const uint64_t /*bytes = nullptr*/, 
	const char* /*id = 0*/)
{
	return Error_Code_Method_Not_Support;
}

void DataSub::pollDataThread()
{
	//1MB
	char* recvbuf{new(std::nothrow) char[1048576]};
	zmq_pollitem_t pollitems[]{ { sso, 0, ZMQ_POLLIN, 0} };
	Sock sock;

	while(recvbuf && sso && !stopped)
	{
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			bool more{true};
			int curpos{0};

			while (more)
			{
				curpos += sock.recv(sso, recvbuf + curpos, 1048576 - curpos, more);
			}

			if (polledDataCallback)
			{
				polledDataCallback(id, recvbuf, curpos, nullptr);
			}
		}
	}
}
