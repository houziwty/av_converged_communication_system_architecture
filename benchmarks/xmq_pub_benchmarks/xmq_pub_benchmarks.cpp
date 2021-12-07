#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "error_code.h"
#include "network/xmq/pub.h"
using namespace framework::network::xmq;

int main(int argc, char* argv[])
{
  const std::string path{argv[0]};
  const int pos{path.rfind('/')};
  const std::string dir{path.substr(0, pos)};
  char* buffer = new char[2 * 1024 * 1024];
  FILE* fd{fopen((dir + "/img102.jpg").c_str(), "rb+")};
  const int jpgbytes{(int)fread(buffer + 12, 1, 2 * 1024 * 1024, fd)};
  fclose(fd);

  Pub pub;
  int ret{pub.start(50927)};
  int sequence{0};

  while (Error_Code_Success == ret)
  {
    const unsigned long long tick{XTime().tickcount()};
    memcpy(buffer, &tick, 8);
    memcpy(buffer + 8, &sequence, 4);
    std::string data{buffer, jpgbytes + 12};
    pub.send(data);

    XTime().sleep(10);
    printf("Pub timestamp = [ %llu ], sequence = [ %d ].\r\n", tick, sequence);
    ++sequence;
  }

  pub.stop();
  return 0;
}