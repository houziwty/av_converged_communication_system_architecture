#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "zmq.h"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "core/dealer.h"
#include "core/sock.h"
#include "worker_task.h"
using namespace module::network::xmq;

WorkerTask::WorkerTask(PolledDataCallback callback) 
	: XMQNode(callback), dso{nullptr}
{}

WorkerTask::~WorkerTask()
{}

int WorkerTask::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		dso = Dealer().connect_inproc(ctx, "inproc://service_dispatcher_task");

		if(dso)
		{
			ret = XMQNode::run(conf, ctx);
		}
	}

	return ret;
}

int WorkerTask::stop()
{
	int ret{XMQNode::stop()};

	if (Error_Code_Success == ret)
	{
		Dealer().shutdown(dso);
	}
	
	return ret;
}

int WorkerTask::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* name/* = nullptr*/)
{
	//这里的data仅表示实际发送的数据
	//不是信封格式
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Operate_Failure};
	int sentbytes{0};

	if (dso && Error_Code_Success == ret)
	{
		Sock sock;
		//先发分隔符
		//再发数据
		const std::string to{name};
		sentbytes += sock.send(dso, to.c_str(), to.length(), true);
		sentbytes += sock.send(dso, "", 0, true);
		uint64_t pos{0};
		while (pos < bytes)
		{
			const uint64_t remain{bytes - pos};
			const uint64_t mtu{remain > 1048576 ? 1048576 : remain};
			sentbytes += sock.send(dso, (uint8_t*)data + pos, mtu, remain > 1048576 ? true : false);
			pos += mtu;
		}
	}

	return sentbytes;
}

void WorkerTask::pollDataThread()
{
	//1MB
	char* recvbuf{new(std::nothrow) char[1048576]{0}};
	zmq_pollitem_t poller[]{ { dso, 0, ZMQ_POLLIN, 0} };
	Sock sock;

	while(recvbuf && dso && !stopped)
	{
		zmq_poll(poller, 1, 1);

		if (poller[0].revents & ZMQ_POLLIN)
		{
			bool more{true};
			std::size_t curpos{0}, headerpos{0};

			while (more)
			{
				int recvbytes{sock.recv(dso, recvbuf + curpos, 1048576 - curpos, more)};

				if (!headerpos && !curpos && 0 < recvbytes)
				{
					headerpos += recvbytes;
				}

				curpos += recvbytes;
			}

			if (polledDataCallback)
			{
				const std::string from{recvbuf, headerpos};
				polledDataCallback(0, (uint8_t*)recvbuf + headerpos, curpos - headerpos, from.c_str());
			}
		}
	}

	boost::checked_array_delete(recvbuf);
}
