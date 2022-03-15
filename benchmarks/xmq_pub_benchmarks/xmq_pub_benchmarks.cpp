#include <cstring>
#include "boost/make_shared.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "error_code.h"
#include "xmq_node.h"
using namespace module::network::xmq;

class TestXMQPub : public XMQNode
{
public:
  TestXMQPub() : XMQNode(){}
  ~TestXMQPub(){}

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
    //     "Fetch forward data = [ %s ] from xmq pub service.", 
    //     msg.c_str());
  }

	void afterFetchOnlineStatusNotification(const bool online = false)
  {
  //  fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test pub service online status = [ %d ].", online);
  }

	void afterFetchServiceCapabilitiesNotification(
		const ServiceInfo* infos = nullptr, 
		const uint32_t number = 0)
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
  char* buffer = new char[4 * 1024 * 1024];
  FILE* fd{fopen((dir + "/img102").c_str(), "rb+")};
  const int jpgbytes{(int)fread(buffer, 1, 4 * 1024 * 1024, fd)};
  fclose(fd);

  const std::string xmq_addr{"192.168.2.172"};
  const std::string name{"test_xmq_pub"};
  boost::shared_ptr<XMQNode> node{
      boost::make_shared<TestXMQPub>()};
  XMQModeConf conf{0};
  conf.id = 0xC2;
  conf.port = 60531;
  conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
  XMem().copy(name.c_str(), name.length(), conf.name, 128);
  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
  node->addConf(conf);

  XMQModeConf conf1{0};
  conf1.id = 0xC9;
  conf1.port = 60927;
  conf1.type = XMQModeType::XMQ_MODE_TYPE_PUB;
  XMem().copy(name.c_str(), name.length(), conf1.name, 128);
  node->addConf(conf1);

  XMQModeConf conf2{0};
  conf2.id = 0xC4;
  conf2.port = 60928;
  conf2.type = XMQModeType::XMQ_MODE_TYPE_PUB;
  XMem().copy(name.c_str(), name.length(), conf2.name, 128);
//  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
  node->addConf(conf2);

  int ret{node->run()}, sequence{0};

  while (Error_Code_Success == ret)
  {
    // const unsigned long long tick{XTime().tickcount()};
    // memcpy(buffer, &tick, 8);
    // memcpy(buffer + 8, &sequence, 4);
    ret = node->send(0xC1, buffer, jpgbytes);

    XTime().sleep(10);
//    printf("Pub timestamp = [ %llu ], sequence = [ %d ], result = [%d].\r\n", tick, sequence, ret);
//    ++sequence;
  }

  node->stop();
  return 0;
}