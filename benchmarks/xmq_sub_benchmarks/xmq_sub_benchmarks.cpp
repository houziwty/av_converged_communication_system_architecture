#include <stdio.h>
#include <string.h>
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
	//订阅消息接收
	//消息体内容由具体数据内容决定
    printf("%s.\r\n", (const char*)data);
  }
};

int main(int argc, char* argv[])
{
  const char* xmq_addr{"127.0.0.1"};//数据发布服务IP地址
  const char* name{"test_xmq_sub"};//任意定义
  //boost::shared_ptr<Libxmq> node{
  //    boost::make_shared<TestXMQSub>()};
  Libxmq* node = new TestXMQSub();
  XMQNodeConf conf{0};
  conf.id = 0xF1;//任意定义
  conf.port = 60001;//数据订阅端口号，暂定
  conf.type = XMQModuleType::XMQ_MODULE_TYPE_SUB;
  //字符串拷贝，请按需使用memcpy即可
//  XMem().copy(name.c_str(), name.length(), conf.name, 128);
  memcpy_s(conf.name, 128, name, strlen(name));
//  XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);
  memcpy_s(conf.ip, 32, xmq_addr, strlen(xmq_addr));
  int ret{node->addNode(conf)}, sequence{0};
  getchar();
  node->removeNode(0xF1);
  return 0;
}