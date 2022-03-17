#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "libvideo_encode_image.h"

int deviceId{-1};
ThreadPool tp;

void getEncodeImageThread(void)
{
  FILE* fd_bgr{nullptr};
  fd_bgr = fopen("/mnt/build/out.bgr", "wb+");

  while (0 < deviceId)
  {
    OutputFrameData* data{nullptr};
    CLT_LIB_VideoEncodeImage_GetFrame(deviceId, data);

    if (data)
    {
      printf("jpeg picture data size = %d, bgr24 frame size = %d, sequence = %d, ts = %f.\r\n", 
      data->jpegbytes, data->bgr24bytes, data->sequence, data->timestamp);

      // char jpgName[256]{0};
      // sprintf(jpgName, "/mnt/build/capture/%d.jpg", data->sequence);
      // FILE* fd_jpg{nullptr};
      // fd_jpg = fopen(jpgName, "wb+");
      // fwrite(data->jpeg, data->jpegbytes, 1, fd_jpg);
      // fclose(fd_jpg);

      // fwrite(data->bgr24, data->bgr24bytes, 1, fd_bgr);

      delete[] (void*)(data->bgr24);
      delete[] (void*)(data->jpeg);
      delete data;
    }
    else
    {
      printf("Invalid output frame data.\r\n");
      XTime().sleep(10);
    }
  }
}

int main()
{
  if(0 == CLT_LIB_VideoEncodeImage_Start())
  {
    deviceId = CLT_LIB_VideoEncodeImage_AddDevice("admin", "Vrc123456", "192.168.2.225", 8000);
    tp.createNew(boost::bind(&getEncodeImageThread));
    getchar();
  }

  return 0;
}