#include <iostream>
#include "boost/make_shared.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "error_code.h"
#include "libxmq.h"
using namespace module::network::xmq;

static const std::string trafficName{"traffic_cooperate_host_server"};

class XMQSendBenchmark : public Libxmq
{
public:
	XMQSendBenchmark() : Libxmq(), stopped{false}, thr{nullptr}
	{}
	~XMQSendBenchmark() {}

private:
	bool stopped;
	void* thr;

public:
	void task_worker_thread(void)
	{
		char buf[629270]{ 0 };
// 		FILE* fd{ nullptr };
// #ifdef _WINDOWS
// 		fopen_s(&fd, "D:\\av_converged_communication_system_architecture\\build\\192.168.2.164_01_20220424161253411.jpeg", "rb+");
// #else
// 		fd = fopen("/mnt/d/av_converged_communication_system_architecture/build/192.168.2.164_01_20220424161253411.jpeg", "rb+");
// #endif//_WINDOWS
// 		const std::size_t bytes = fread(buf, 629270, 1, fd);

		uint64_t count{0};
		const std::size_t sndbytes{ 1024 };
		char* sndbuf = new char[sndbytes]{0};
		memset(sndbuf, 0, sndbytes);
//		printf(sndbuf, "config://traffic_cooperate_host_server?data={\"command\":\"send_test\",\"text\":%s}", buf);
		const std::string text{
			"config://traffic_cooperate_host_server?data={\"command\":\"send_test\",\"text\":\"1234\"}"};

		while (!stopped)
		{
			int ret = Libxmq::send(0xF1, text.c_str(), text.length(), nullptr);

			if (ret)
			{
				std::cout << "Send result = " << ret << ", count = " << ++count << std::endl;
			}

			XTime().sleep(1000);
		}
	}

protected:
	void afterPolledXMQDataNotification(
		const uint32_t id = 0,
		const void* data = nullptr,
		const uint64_t bytes = 0,
		const char* name = nullptr)
	{
		const std::string msg{ reinterpret_cast<const char*>(data), bytes };

		printf("%s\r\n", msg.c_str());
	}

	void afterFetchOnlineStatusNotification(const bool online = false)
	{
		if (online)
		{
			std::cout << "benchmark thread online." << std::endl;
			thr = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&XMQSendBenchmark::task_worker_thread, this));
		}
		else
		{
			std::cout << "benchmark thread offline." << std::endl;
			stopped = true;
			ThreadPool().get_mutable_instance().destroy(thr);
		}
	}

	void afterFetchServiceCapabilitiesNotification(const char* names = nullptr)
	{
		std::cout << names << std::endl;
	}
};

int main(int argc, char* argv[])
{
	const std::string xmq_addr{ "192.168.2.53" };
	const std::string name{ "xmq_send_benchmark" };
	XMQSendBenchmark benchmark;
	XMQNodeConf conf{ 0 };
	conf.id = 0xF1;
	conf.port = 60531;
	conf.type = XMQModuleType::XMQ_MODULE_TYPE_TASK;
	XMem().copy(trafficName.c_str(), trafficName.length(), conf.name, 128);
	XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
	// conf.opt.hwm = 3;
	// conf.opt.reconivl = 5000;

	benchmark.addNode(conf);
	getchar();
	benchmark.removeNode(conf.id);

	return 0;
}