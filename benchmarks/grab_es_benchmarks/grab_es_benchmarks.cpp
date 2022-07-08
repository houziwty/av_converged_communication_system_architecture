#include <string>
#include <cstring>
#include <functional>
#include "libasio.h"
using namespace module::network::asio;
#include "libav.h"
#include "libavpkt.h"
using namespace module::av::stream;
#include "time/xtime.h"
using namespace framework::utils::time;

class GrabESBenchmarks : public Libasio, public Libav
{
public:
  GrabESBenchmarks() : Libasio(), Libav(), connected{false}
  {
    AVModeConf conf;
    conf.id = 100;
    conf.type = AVModeType::AV_MODE_TYPE_GRAB_ES;
    conf.hwnd = nullptr;
    conf.infos = nullptr;
    conf.callback = std::bind(&GrabESBenchmarks::afterGotESFrameCallback, this, std::placeholders::_1, std::placeholders::_2);
    Libav::addConf(conf);
  }
  ~GrabESBenchmarks(){}

public:
  bool connected;

protected:
  uint32_t afterFetchIOAcceptedEventNotification(
    const char* remoteIP = nullptr, 
    const uint16_t remotePort = 0, 
    const uint16_t localPort = 0, 
    const int32_t e = 0) override
  {
    return 0;
  }

  uint32_t afterFetchIOConnectedEventNotification(
    const int32_t e = 0, 
    void* user = nullptr)
  {
    connected = true;
    return 1;
  }

  void afterPolledIOReadDataNotification(
    const uint32_t id = 0, 
    const void* data = nullptr, 
    const uint64_t bytes = 0, 
    const int32_t e = 0) override
  {
//    printf("id = %d, bytes = %d.\r\n", id, bytes);
    Libavpkt pkt;
    pkt.input(data, bytes);
    Libav::input(100, &pkt);
  }

  void afterPolledIOSendDataNotification(
    const uint32_t id = 0, 
    const uint64_t bytes = 0, 
    const int32_t e = 0)
  {

  }

  void afterGotESFrameCallback(const uint32_t id, const void* pkt)
  {
    Libavpkt* avpkt{(Libavpkt*)pkt};
    printf("afterGotESFrameCallback id = %d, bytes = %d.\r\n", id, avpkt->bytes());

    static FILE* fd{nullptr};
    if(!fd)
    {
      fd = fopen("/mnt/d/test.264", "wb+");
    }
    fwrite(avpkt->data(), avpkt->bytes(), 1, fd);
  }
};//class GrabESBenchmarks

int main(int argc, char* argv[])
{
  Libasio* node = new GrabESBenchmarks();
  ASIOModeConf conf;
  conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
  conf.port = 60820;
  conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
  conf.tcp.ip = "192.168.2.160";
  conf.tcp.user = nullptr;

  int ret{node->addConf(conf)};
  GrabESBenchmarks* benchmarks{(GrabESBenchmarks*)node};
  const std::string uri{"realplay://1?data={\"channel\":\"1\",\"command\":\"1\",\"stream\":\"0\"}"};
  const uint64_t bytes{ 32 + uri.length() };
  char* data{ new char[bytes] };
  *((uint32_t*)data) = 0xFF050301;
  *((uint32_t*)(data + 4)) = 0;
  *((uint32_t*)(data + 8)) = 0;
  *((uint32_t*)(data + 12)) = uri.length();
  *((uint32_t*)(data + 16)) = 0;
  *((uint64_t*)(data + 24)) = 0;
  memcpy(data + 32, uri.c_str(), uri.length());

  while (1)
  {
    if (!benchmarks->connected)
    {
      XTime().sleep(3000);
      continue;
    }

    if(!node->send(1, data, bytes));
    {
      break;
    }
  }
  
  getchar();
  node->removeConf(0);
  return 0;
}