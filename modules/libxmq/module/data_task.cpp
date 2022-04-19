#include "boost/format.hpp"
#include "zmq.h"
#include "error_code.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url.h"
using namespace framework::utils::data;
#include "core/dealer.h"
#include "core/sock.h"
#include "data_task.h"
using namespace module::network::xmq;

DataTask::DataTask(
	PolledDataCallback poll, 
	CheckOnlineStatusCallback check, 
	ServiceCapabilitiesNotificationCallback capabilities) 
	: XMQNode(poll), registerRepTickout{XTime().tickcount()}, online{false}, check{nullptr}, dso{nullptr}, 
	checkOnlineStatusCallback{check}, serviceCapabilitiesNotificationCallback{capabilities}, id{0}
{}

DataTask::~DataTask()
{}

int DataTask::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		dso = Dealer().connect(
			ctx, conf.name, conf.ip, conf.port, conf.opt.hwm, conf.opt.reconivl);

		if(dso && Error_Code_Success == XMQNode::run(conf, ctx))
		{
			id = conf.id;
			check = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&DataTask::checkOnlineThread, this, conf.name));
		}
	}

	return ret;
}

int DataTask::stop()
{
	int ret{XMQNode::stop()};

	if (Error_Code_Success == ret)
	{
		ThreadPool().get_mutable_instance().destroy(check);
		Dealer().shutdown(dso);
	}
	
	return ret;
}

int DataTask::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* /*name = nullptr*/)
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

void DataTask::pollDataThread()
{
	//1MB
	char* recvbuf{new(std::nothrow) char[1048576]};
	zmq_pollitem_t pollers[]{ { dso, 0, ZMQ_POLLIN, 0} };
	Sock sock;

	while(dso && !stopped)
	{
		zmq_poll(pollers, 1, 1);

		if (pollers[0].revents & ZMQ_POLLIN)
		{
			bool more{true};
			int curpos{0}, headerpos{0};

			while (more)
			{
				curpos += sock.recv(dso, recvbuf + curpos, 1048576 - curpos, more);
			}

			Url url;
			int ret{ url.parse(recvbuf, curpos)};

			if(Error_Code_Success == ret)
			{
				if (!url.proto().compare("register"))
				{
					const std::vector<Parameter> items{url.parameters()};

					for(int i = 0; i != items.size(); ++i)
					{
						if(!items[i].key.compare("timestamp"))
						{
							registerRepTickout = std::stoull(items[i].value);
							if(checkOnlineStatusCallback && XTime().tickcount() - registerRepTickout < 90000 && !online)
							{
								online = true;
								checkOnlineStatusCallback(online);
							}

							if (online)
							{
								const std::string msg{
									(boost::format("query://%s?timestamp=%lld") % XMQHostID % XTime().tickcount()).str()};
								send(msg.c_str(), msg.length());
							}
							break;
						}
					}
				}
				else if (!url.proto().compare("query") && 
						!url.host().compare(XMQHostID))
				{
					const std::vector<Parameter> parameters{url.parameters()};
					const std::size_t number{parameters.size()};

					if (0 < number)
					{
						uint32_t count{0};
						char* names[300]{0};

						for (int i = 0; i != number; ++i)
						{
							if(!parameters[i].key.compare("name"))
							{
								names[i] = (char*)parameters[i].value.c_str();
								++count;
							}
						}

						if (serviceCapabilitiesNotificationCallback)
						{
							serviceCapabilitiesNotificationCallback((const char**)names, count);
						}
					}
				}
				else
				{
					if (polledDataCallback)
					{
						polledDataCallback(id, recvbuf, curpos, nullptr);
					}
				}
			}
		}
	}

	boost::checked_array_delete(recvbuf);
}

void DataTask::checkOnlineThread(const char* name/* = nullptr*/)
{
	int sequence{1};
	uint64_t lastTickcount{0};
	XTime xt;
	Sock sock;

	while (!stopped)
	{
		uint64_t curTickcount{xt.tickcount()};

		if(curTickcount - lastTickcount > 30000)
		{
			//Register and heartbeat
			const std::string url{
				(boost::format("register://%s?timestamp=%lld&sequence=%lld") % name % curTickcount % sequence++).str()};

			if(!url.empty())
			{
				send(url.c_str(), url.length());
			}

			lastTickcount = curTickcount;
		}

		if(checkOnlineStatusCallback && curTickcount - registerRepTickout >= 90000 && online)
		{
			online = false;
			checkOnlineStatusCallback(online);
		}

		xt.sleep(1000);
	}
}