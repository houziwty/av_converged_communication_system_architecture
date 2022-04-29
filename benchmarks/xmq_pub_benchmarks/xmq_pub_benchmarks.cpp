#include <cstring>
#include "boost/make_shared.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "error_code.h"
#include "libxmq.h"
using namespace module::network::xmq;

class TestXMQPub : public Libxmq
{
public:
  TestXMQPub() : Libxmq(){}
  ~TestXMQPub(){}

protected:
	void afterPolledXMQDataNotification(
		const uint32_t id = 0, 
    const void* data = nullptr,  
    const uint64_t bytes = 0, 
    const char* from = nullptr)
  {
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    // fileLog.write(
    //     SeverityLevel::SEVERITY_LEVEL_INFO, 
    //     "Fetch forward data = [ %s ] from xmq pub service.", 
    //     msg.c_str());
  }

	void afterFetchOnlineStatusNotification(const bool online = false)
  {
  //  fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test pub service online status = [ %d ].", online);
  }

	void afterFetchServiceCapabilitiesNotification(const char** names = nullptr)
  {
    // fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq pub service capabilities size = [ %d ].", number);

    // for(int i = 0; i != number; ++i)
    // {
    //     fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", infos[i].name);
    // }
  }
};

int main(int argc, char* argv[])
{
//  fileLog.createNew(argv[0]);
  const std::string path{argv[0]};
  const int pos{(int)path.rfind('/')};
  const std::string dir{path.substr(0, pos)};
  char* buffer = new char[1024 * 1024];
  FILE* fd{fopen("/mnt/d/av_converged_communication_system_architecture/build/192.168.2.164_01_20220424161253411.jpeg", "rb+")};
  const int jpgbytes{(int)fread(buffer, 1, 1024 * 1024, fd)};
  fclose(fd);

  const std::string xmq_addr{"127.0.0.1"};
  const std::string name{"test_xmq_pub"};
  boost::shared_ptr<Libxmq> node{
      boost::make_shared<TestXMQPub>()};
  XMQNodeConf conf{0};
  conf.id = 0xC2;
  conf.port = 60001;
  conf.type = XMQModuleType::XMQ_MODULE_TYPE_PUB;
  XMem().copy(name.c_str(), name.length(), conf.name, 128);
  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

  int ret{node->addNode(conf)}, sequence{0};

  while (1)
  {
    // const unsigned long long tick{XTime().tickcount()};
    // memcpy(buffer, &tick, 8);
    // memcpy(buffer + 8, &sequence, 4);
    ret = node->send(0xC2, buffer, jpgbytes);

    XTime().sleep(1000);
//    printf("Pub timestamp = [ %llu ], sequence = [ %d ], result = [%d].\r\n", tick, sequence, ret);
//    ++sequence;
  }

  node->removeNode(0xC2);
  return 0;
}