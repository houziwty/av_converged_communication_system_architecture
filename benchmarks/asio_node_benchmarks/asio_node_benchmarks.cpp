#include "boost/make_shared.hpp"
#include "error_code.h"
#include "asio_node.h"
using namespace module::network::asio;
#include "log.h"
using namespace module::file::log;

FileLog fileLog;

class TestUdpNode : public ASIONode
{
public:
  TestUdpNode() : ASIONode(){}
  ~TestUdpNode(){}

public:
	int run(void)
  {
    return ASIONode::run();
  }

	int stop(void) override
  {
    return ASIONode::stop();
  }

protected:
  uint32_t afterFetchAcceptedEventNotification(
    const char* ip = nullptr, 
    const uint16_t port = 0, 
    const int32_t e = 0) override
  {
    return 0;
  }

  uint32_t afterFetchConnectedEventNotification(const int32_t e = 0) override
  {
    return 0;
  }

	void afterPolledReadDataNotification(
		const uint32_t id = 0, 
    const void* data = nullptr, 
    const uint64_t bytes = 0, 
    const int32_t e = 0) override
  {
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Polled UDP data = [ %s ], len = [ %llu ] from session = [ %u ].", 
        (const char*)data, bytes, id);
  }

  void afterPolledSendDataNotification(
    const uint32_t id = 0, 
    const uint64_t bytes = 0, 
    const int32_t e = 0) override
  {
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Sent UDP data len = [ %llu ] from session = [ %u ].", bytes, id);
  }
};

int main(int argc, char* argv[])
{
  fileLog.createNew(argv[0]);
  
  boost::shared_ptr<ASIONode> node{
      boost::make_shared<TestUdpNode>()};

  int ret{node->run()}, number{0};
  ASIOModeConf conf;
  conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_UDP;
  conf.port = 60001;
  conf.udp.sid = 1;
  node->addConf(conf);
  const std::string data{"Hello world!"};

  while (1)
  {
    node->send(1, data.c_str(), data.length(), "127.0.0.1", 60000);
  }
  
  getchar();
  node->stop();
  return 0;
}