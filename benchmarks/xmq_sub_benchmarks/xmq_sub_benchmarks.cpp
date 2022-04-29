#include "boost/make_shared.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "error_code.h"
#include "libxmq.h"
using namespace module::network::xmq;

class TestXMQSub : public Libxmq
{
public:
  TestXMQSub() : Libxmq(){}
  ~TestXMQSub(){}

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
    //     "Fetch forward data = [ %s ] from xmq sub service.", 
    //     msg.c_str());

    printf("%s.\r\n", msg.c_str());
  }

	void afterFetchOnlineStatusNotification(const bool online = false)
  {
//    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test sub service online status = [ %d ].", online);
  }

	void afterFetchServiceCapabilitiesNotification(
		const char** names = nullptr)
  {
    // fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq sub service capabilities size = [ %d ].", number);

    // for(int i = 0; i != number; ++i)
    // {
    //     fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", infos[i].name);
    // }
  }
};

int main(int argc, char* argv[])
{
  const std::string xmq_addr{"127.0.0.1"};
  const std::string name{"test_xmq_sub"};
  boost::shared_ptr<Libxmq> node{
      boost::make_shared<TestXMQSub>()};
  XMQNodeConf conf{0};
  conf.id = 0xF1;
  conf.port = 60001;
  conf.type = XMQModuleType::XMQ_MODULE_TYPE_SUB;
  XMem().copy(name.c_str(), name.length(), conf.name, 128);
  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
  int ret{node->addNode(conf)}, sequence{0};
  getchar();
  node->removeNode(0xF1);
  return 0;
}