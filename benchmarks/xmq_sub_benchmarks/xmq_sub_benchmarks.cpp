#include "error_code.h"
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "network/xmq/sub.h"
using namespace framework::network::xmq;

class XSub final : public Sub
{
public:
  XSub(void) : Sub(), count{0}
  {}

  ~XSub(void)
  {}

protected:
  void afterWorkerPolledDataHandler(const std::string data)
  {
    const unsigned long long tick{XTime().tickcount()};
    const char* buffer{data.c_str()};
    unsigned long long* send_ts{(unsigned long long*)buffer};
    int* sequence{(int*)(buffer + 8)};
    const int diff{(int)(tick - *send_ts)};

  //  if(10 < diff)
    {
      printf("send_ts = [ %llu ], sequence = [ %d ], diff = [ %d ].\r\n", *send_ts, *sequence, diff);
    }

    // if(0 == (count++ % 10000))
    // {
    //   printf("send_ts = [ %llu ], bytes = [ %d ], diff = [ %d ].\r\n", *send_ts, *bytes, diff);
    // }
  }

private:
  int count;
};//class XSub

int main(int argc, char* argv[])
{
  XSub xsub;
  int ret{xsub.start("127.0.0.1", 50927)};
  getchar();
  xsub.stop();
  return 0;
}