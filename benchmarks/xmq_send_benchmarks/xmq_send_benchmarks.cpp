#include <iostream>
#include "boost/make_shared.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "thread/thread.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "error_code.h"
#include "xmq_node.h"
using namespace module::network::xmq;

class XMQSendBenchmark : public XMQNode
{
public:
	XMQSendBenchmark() : XMQNode() {}
	~XMQSendBenchmark() {}

public:
	int run(void)
	{
		return XMQNode::run();
	}

	int stop(void) override
	{
		return XMQNode::stop();
	}

	void task_worker_thread(void)
	{
		//char buf[3 * 1024 * 1024]{ 0 };
		//FILE* fd{ nullptr };
		//fopen_s(&fd, "C:\\Users\\aaa\\OneDrive\\Desktop\\192.168.2.226_01_20220406090609329.jpeg", "rb+");
		//const std::size_t bytes = fread(buf, 600 * 1024, 1, fd);

		const std::size_t bytes{ 800 * 1024 };
		char* buf = new char[bytes];
		memset(buf, 1, bytes);
		unsigned long long start{ XTime().tickcount() };

		while (1)
		{
			int ret = XMQNode::send(0xF1, buf, bytes, nullptr);
			unsigned long long stop{ XTime().tickcount() };

			if (!ret)
			{
				//std::cout << "Send result = " << ret << ", expire = " << stop - start << std::endl;
			}

			start = stop;

			XTime().sleep(5);
		}
	}

protected:
	void afterPolledDataNotification(
		const uint32_t id = 0,
		const void* data = nullptr,
		const uint64_t bytes = 0,
		const char* from = nullptr)
	{
		const std::string msg{ reinterpret_cast<const char*>(data), bytes };
	}

	void afterFetchOnlineStatusNotification(const bool online = false)
	{
		if (online)
		{
			ThreadPool().get_mutable_instance().createNew(
				boost::bind(&XMQSendBenchmark::task_worker_thread, this));
		}
	}

	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr,
		const uint32_t number = 0)
	{
		std::string text;
		for (int i = 0; i != number; ++i)
		{
			text += ("[ " + std::string(infos[i].name) + " ]");
		}
		std::cout << text << std::endl;
	}
};

int main(int argc, char* argv[])
{
	const std::string xmq_addr{ "192.168.2.172" };
	const std::string name{ "xmq_send_benchmark" };
	boost::shared_ptr<XMQNode> node{
		boost::make_shared<XMQSendBenchmark>() };
	XMQModeConf conf{ 0 };
	conf.id = 0xF1;
	conf.port = 60931;
	conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
	XMem().copy(name.c_str(), name.length(), conf.name, 128);
	XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
	node->addConf(conf);
	int ret{ node->run() }, sequence{ 0 };
	getchar();
	node->stop();

	return 0;
}