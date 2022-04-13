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
		// char buf[3 * 1024 * 1024]{ 0 };
		// FILE* fd{ nullptr };
		// fopen(&fd, "C:\\Users\\aaa\\OneDrive\\Desktop\\192.168.2.230_01_20220413082310260.jpeg", "rb+");
		// const std::size_t bytes = fread(buf, 600 * 1024, 1, fd);

		uint64_t count{0};
		const std::size_t bytes{ 800 * 1024 };
		char* buf = new char[bytes]{0};
		memset(buf, 0, bytes);
		unsigned long long start{ XTime().tickcount() };

		while (!stopped)
		{
			uint64_t* number{(uint64_t*)buf};
			*number = ++count;

			int ret = Libxmq::send(0xF1, buf, bytes, nullptr);
			unsigned long long stop{ XTime().tickcount() };

			if (ret)
			{
				std::cout << "Send result = " << count << ", expire = " << stop - start << std::endl;
			}

			start = stop;

			XTime().sleep(40);
		}
	}

protected:
	void afterPolledDataNotification(
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

	void afterFetchServiceCapabilitiesNotification(
		const char** infos = nullptr,
		const uint32_t number = 0)
	{
		std::string text;
		for (int i = 0; i != number; ++i)
		{
			text += ("[ " + std::string(infos[i]) + " ]");
		}
		std::cout << text << std::endl;
	}
};

int main(int argc, char* argv[])
{
	const std::string xmq_addr{ "192.168.2.172" };
	const std::string name{ "xmq_send_benchmark" };
	XMQSendBenchmark benchmark;
	XMQNodeConf conf{ 0 };
	conf.id = 0xF1;
	conf.port = 60931;
	conf.type = XMQModuleType::XMQ_MODULE_TYPE_TASK;
	XMem().copy(name.c_str(), name.length(), conf.name, 128);
	XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
	conf.opt.hwm = 3;
	conf.opt.reconivl = 5000;

	benchmark.addNode(conf);
	getchar();
	benchmark.removeNode(conf.id);

	return 0;
}