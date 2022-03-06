#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "asio_node.h"
using namespace module::network::asio;
#include "http_node.h"
using namespace module::network::http;
#include "log.h"
using namespace module::file::log;

FileLog fileLog;

class HttpServerNode 
  : public ASIONode, protected HttpNode
{
public:
  HttpServerNode() : ASIONode(), HttpNode(), sid{0}
  {}
  ~HttpServerNode(){}

public:

protected:
  uint32_t afterFetchAcceptedEventNotification(
    const char* ip = nullptr, 
    const uint16_t port = 0, 
    const int32_t e = 0) override
  {
    int ret{!e ? ++sid : 0};

    if (!e)
    {
      HTTPModeConf conf;
      conf.id = sid;
      const std::string addr{ip};
      XMem().copy(ip, addr.length(), conf.ip, 32);
      conf.port = port;
      ret = HttpNode::addConf(conf);

      if (Error_Code_Success == ret)
      {
        fileLog.write(
          SeverityLevel::SEVERITY_LEVEL_INFO, 
          "Add new http session from = [ %s ], port = [ %d ] with id = [ %u ] successfully.", 
          ip, port, sid);
      }
      else
      {
        fileLog.write(
          SeverityLevel::SEVERITY_LEVEL_WARNING, 
          "Add new http session from = [ %s ], port = [ %d ] with id = [ %u ] failed, result = [ %d ].", 
          ip, port, sid, ret);
      }
    }

    return sid;
  }

  uint32_t afterFetchConnectedEventNotification(const int32_t e = 0) override
  {
    return Error_Code_Method_Not_Support;
  }

	void afterPolledReadDataNotification(
		const uint32_t id = 0, 
    const void* data = nullptr, 
    const uint64_t bytes = 0, 
    const int32_t e = 0) override
  {
    if (!e && 0 < id && data && 0 < bytes)
    {
      fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Polled data = [ %s ], bytes = [ %d ] from session id = [ %u ] successfully.", 
        (const char*)data, bytes, id);

      HttpNode::input(id, e, data, bytes);
    }
    else
    {
      HttpNode::removeConf(id);
      fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_WARNING, 
        "Polled data = [ %s ], bytes = [ %d ] from session id = [ %u ] failed, result = [ %d ].", 
        (const char*)data, bytes, e);
    }
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

private:
  uint32_t sid;
};

int main(int argc, char* argv[])
{
  fileLog.createNew(argv[0]);
  
  boost::shared_ptr<ASIONode> node{
      boost::make_shared<HttpServerNode>()};

  int ret{node->run()}, number{0};
  ASIOModeConf conf;
  conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
  conf.port = 8080;
  conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
  node->addConf(conf);  
  getchar();
  node->stop();
  return 0;
}