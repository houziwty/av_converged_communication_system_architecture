#include "libvideo_encode_image.h"

int main()
{
  if(0 == CLT_LIB_VideoEncodeImage_Start())
  {
    int ret{CLT_LIB_VideoEncodeImage_AddDevice("admin", "Vrc123456", "192.168.2.225", 8000)};
    getchar();
  }

  return 0;
}