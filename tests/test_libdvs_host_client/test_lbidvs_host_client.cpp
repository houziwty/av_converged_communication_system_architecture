#include "libdvs_host_client/libdvs_host_client.h"

int main()
{
  DvsHostClient sdk;
  sdk.connectXMQService("xmq_host_service", "127.0.0.1", 50531);
  return 0;
}