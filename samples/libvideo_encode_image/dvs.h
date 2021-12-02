//
//		Copyright :						@2021, 车路通, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-05
//		Description :					设备类
//
//		History:						Author									Date									Description
//										王科威									 2021-11-05								 创建
//

#ifndef CLT_DVS_H
#define CLT_DVS_H

#include <string>
#include <vector>
#include <mutex>
#include "HCNetSDK.h"
#include "LinuxPlayM4.h"
//#include "boost/thread.hpp"
//#include "boost/thread/mutex.hpp"
extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include "libvideo_encode_image.h"
#include "JPEGImageEncoder.h"
#include "utils/fifo/fifo.h"

typedef struct tagH264Frame_t
{
    void* buffer;
    int bytes;
    int type;
}H264Frame;

class Dvs
{
public:
    Dvs(const int did);
    ~Dvs(void);

public:
    int login(
        const std::string ip, 
        const unsigned short port, 
        const std::string name, 
        const std::string password);
    void getOneFrameData(OutputFrameData*& data);

private:
    static void CALLBACK RealDataCallBack_V30(
		LONG lRealHandle,
		DWORD dwDataType,
		BYTE* pBuffer,
		DWORD dwBufSize,
		void* dwUser);
    static void CALLBACK DecodeOneFrameCallback(
		int nPort, 
        char* pBuf, 
        int nSize, 
        FRAME_INFO* pFrameInfo, 
        void* nUser, 
        int nReserved2);
    void decodeThreadHandler(void);


private:
    const int deviceId;
    static int deviceCount;
    int userID;
    int realplayID;
    int playID;
    char mac[17];
 //   boost::thread_group tg;
    bool stopped;
    std::vector<H264Frame> frames;
	std::mutex mtx;
    AVFrame* iframe;
	AVFrame* oframe;
	void* yuvframebuf;
	void* bgrframebuf;
    struct SwsContext* ctx;
    int yuvframebufsize;
    int bgrframebufsize;
    JPEGImageEncoder jpegEncoder;
    Fifo<OutputFrameData*> frameDataQueue;
    int sequence;
};//class Dvs

#endif//CLT_DVS_H