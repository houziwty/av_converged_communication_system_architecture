#include "boost/make_shared.hpp"
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "error_code.h"
#include "xmq_node.h"
using namespace module::network::xmq;

class TestXMQSub : public XMQNode
{
public:
  TestXMQSub() : XMQNode(){}
  ~TestXMQSub(){}

public:
	int run(void)
  {
    return XMQNode::run();
  }

	int stop(void) override
  {
    return XMQNode::stop();
  }

protected:
	void afterPolledDataNotification(
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
  }

	void afterFetchOnlineStatusNotification(const bool online = false)
  {
//    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test sub service online status = [ %d ].", online);
  }

	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr, 
		const uint32_t number = 0)
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
  const std::string xmq_addr{"192.168.2.72"};
  const std::string name{"test_xmq_sub"};
  boost::shared_ptr<XMQNode> node{
      boost::make_shared<TestXMQSub>()};
  XMQModeConf conf1{0};
  conf1.id = 0xE1;
  conf1.port = 60927;
  conf1.type = XMQModeType::XMQ_MODE_TYPE_SUB;
   XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf1.ip, 32);
  XMem().copy(name.c_str(), name.length(), conf1.name, 128);
  node->addConf(conf1);

  XMQModeConf conf2{0};
  conf2.id = 0xF1;
  conf2.port = 60531;
  conf2.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
  XMem().copy(name.c_str(), name.length(), conf2.name, 128);
  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf2.ip, 32);
  node->addConf(conf2);
  int ret{node->run()}, sequence{0};
  getchar();
  node->stop();
  return 0;
}