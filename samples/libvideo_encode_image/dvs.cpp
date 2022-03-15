//#include <cstring>
//#include "boost/thread.hpp"
#include <functional>
#include <thread>
#include <chrono>
#include "memory/xmemcpy.h"
using namespace framework::utils::memory;
#include "dvs.h"

int Dvs::deviceCount = 0;

Dvs::Dvs(const int did) 
    : deviceId{did}, userID{-1}, realplayID{-1}, playID{-1}, stopped{true}, 
    ctx{nullptr}, iframe{nullptr}, oframe{nullptr}, yuvframebuf{nullptr}, bgrframebuf{nullptr}, sequence{0}, frameDataQueue{25}, yuvframebufsize{0},
    bgrframebufsize{0}
{
    if (!deviceCount)
    {
        NET_DVR_Init();
    }

    ++deviceCount;
    memset(mac, 0, 17);
    iframe = av_frame_alloc();
    oframe = av_frame_alloc();
}

Dvs::~Dvs()
{
    stopped = true;
 //   tg.join_all();
    --deviceCount;
    av_frame_free(&iframe);
    av_frame_free(&oframe);
    sws_freeContext(ctx);

    if (!deviceCount)
    {
        NET_DVR_Cleanup();
    }
}

int Dvs::login(
    const std::string ip, 
    const unsigned short port, 
    const std::string name, 
    const std::string password)
{
    NET_DVR_DEVICEINFO_V30 struDeviceInfo{ 0 };
    userID = NET_DVR_Login_V30(
        (char*)ip.c_str(), port, (char*)name.c_str(), (char*)password.c_str(), &struDeviceInfo);

    if (-1 < userID)
    {
        DWORD ret = 0;
        // NET_DVR_NETCFG_V30 cfg;
        // memset(&cfg, 0, sizeof(NET_DVR_NETCFG_V30));
        // if (NET_DVR_GetDVRConfig(userID, NET_DVR_GET_NETCFG_V30, 0,&cfg, sizeof(NET_DVR_NETCFG_V30), &ret))
        // {
        //     sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
        //         cfg.struEtherNet[0].byMACAddr[0],
        //         cfg.struEtherNet[0].byMACAddr[1],
        //         cfg.struEtherNet[0].byMACAddr[2],
        //         cfg.struEtherNet[0].byMACAddr[3],
        //         cfg.struEtherNet[0].byMACAddr[4],
        //         cfg.struEtherNet[0].byMACAddr[5]);
        // }

        NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
        struPlayInfo.hPlayWnd = 0;
        struPlayInfo.lChannel = struDeviceInfo.byStartChan;
        struPlayInfo.byProtoType = 1;
        struPlayInfo.bBlocked = 1;
        realplayID = NET_DVR_RealPlay_V40(userID, &struPlayInfo, &Dvs::RealDataCallBack_V30, this);

        if (-1 < realplayID)
        {
     //       tg.create_thread(boost::bind(&Dvs::decodeThreadHandler, this));

            std::thread decode_thread(std::bind(&Dvs::decodeThreadHandler, this));
            decode_thread.detach();
        }
    }

//    printf("ip=%s, port=%d, name=%s, password=%s, user=%d, realplay=%d, serialNo=%s.\r\n", 
//        ip.c_str(), port, name.c_str(), password.c_str(), userID, realplayID, struDeviceInfo.sSerialNumber);

    return realplayID;
}

void Dvs::RealDataCallBack_V30(
	LONG lRealHandle,
	DWORD dwDataType,
	BYTE* pBuffer,
	DWORD dwBufSize,
	void* dwUser)
{
	Dvs* dvs{ reinterpret_cast<Dvs*>(dwUser) };

	if (dvs)
	{
//        printf("realplay=%d, bytes=%d, type=%d.\r\n", (int)lRealHandle, (int)dwBufSize, (int)dwDataType);

        // static FILE* fd{nullptr};

        // if (!fd)
        // {
        //     fd = fopen("/mnt/bin/x64/Debug/h264file.264", "wb+");
        // }
        // fwrite(pBuffer, dwBufSize, 1, fd);
        

        H264Frame frame{0};
        frame.buffer = new char[dwBufSize];
        memcpy(frame.buffer, pBuffer, dwBufSize);
        frame.bytes = dwBufSize;
        frame.type = dwDataType;
        
        dvs->mtx.lock();
        dvs->frames.push_back(frame);
        dvs->mtx.unlock();
	}
}

void Dvs::decodeThreadHandler()
{
    stopped = false;
    H264Frame frame{0};

    while(!stopped)
    {
        if(!frames.size())
        {
            //boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        mtx.lock();
        frame = frames.front();
        frames.erase(frames.begin());
        mtx.unlock();

        switch (frame.type)
        {
        case NET_DVR_SYSHEAD:
            if (!PlayM4_GetPort(&playID))
            {
                break;
            }

            if (frame.bytes > 0)
            {
                if (!PlayM4_SetStreamOpenMode(playID, STREAME_REALTIME))
                {
                    break;
                }
                if(!PlayM4_SetDecCallBackExMend(playID, &Dvs::DecodeOneFrameCallback, NULL, 0, this))
                {
                    break;
                }
                if (!PlayM4_OpenStream(playID, (unsigned char *)frame.buffer, frame.bytes, 1024 * 1024))
                {
                    break;
                }
                if (!PlayM4_Play(playID, NULL))
                {
                    break;
                }
            }
            break;
        case NET_DVR_STREAMDATA:
            if (frame.bytes > 0 && playID != -1)
            {
                if (!PlayM4_InputData(playID, (unsigned char *)frame.buffer, frame.bytes))
                {
                    break;
                }
            }
            break;
        default:
            if (frame.bytes > 0 && playID != -1)
            {
                if (!PlayM4_InputData(playID, (unsigned char *)frame.buffer, frame.bytes))
                {
                    break;
                }
            }
            break;
        }

        if(frame.buffer)
        {
            delete[] (unsigned char*)frame.buffer;
        }
    }
}

void Dvs::DecodeOneFrameCallback(
    int nPort, 
    char* pBuf, 
    int nSize, 
    FRAME_INFO* pFrameInfo, 
    void* nUser, 
    int nReserved2)
{
    Dvs* dvs{
        reinterpret_cast<Dvs*>(nUser) };

    if (T_YV12 == pFrameInfo->nType)
    {
        if(!dvs->ctx)
        {
            dvs->ctx = sws_getContext(
                pFrameInfo->nWidth, pFrameInfo->nHeight, AV_PIX_FMT_YUV420P, 
                pFrameInfo->nWidth, pFrameInfo->nHeight, AV_PIX_FMT_BGR24,
                SWS_BILINEAR, 0, 0, 0);
            dvs->bgrframebufsize = av_image_get_buffer_size(AV_PIX_FMT_BGR24, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
            dvs->bgrframebuf = av_malloc(dvs->bgrframebufsize);
            av_image_fill_arrays(
                dvs->oframe->data, dvs->oframe->linesize, (const void*)dvs->bgrframebuf,
                AV_PIX_FMT_BGR24, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
            dvs->yuvframebufsize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
            dvs->yuvframebuf = av_malloc(dvs->yuvframebufsize);
        }
        int ysize{pFrameInfo->nWidth * pFrameInfo->nHeight};
        int uvsize{ysize / 4};
        XMemory().copy(pBuf, dvs->yuvframebuf, ysize);
        XMemory().copy(pBuf + ysize, dvs->yuvframebuf + ysize + uvsize, uvsize);
        XMemory().copy(pBuf + ysize + uvsize, dvs->yuvframebuf + ysize, uvsize);

        av_image_fill_arrays(
            dvs->iframe->data, dvs->iframe->linesize, (const void*)dvs->yuvframebuf,
            AV_PIX_FMT_YUV420P, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
        void* temp{dvs->iframe->data[1]};
            dvs->iframe->data[1] = dvs->iframe->data[2];
            dvs->iframe->data[2] = temp;
        sws_scale(
            dvs->ctx, (uint8_t const* const*)dvs->iframe->data, dvs->iframe->linesize, 0, 
            pFrameInfo->nHeight, dvs->oframe->data, dvs->oframe->linesize);

        void* jpeg{nullptr};
        int jpegbytes{0};
        dvs->jpegEncoder.input(dvs->yuvframebuf, pFrameInfo->nWidth, pFrameInfo->nHeight, jpeg, jpegbytes);
        OutputFrameData* ofd{new(std::nothrow) OutputFrameData};

        if(ofd && 25 > dvs->frameDataQueue.size())
        {
            ofd->bgr24 = new uint8_t[dvs->bgrframebufsize];
            XMemory().copy(dvs->bgrframebuf, ofd->bgr24, dvs->bgrframebufsize);
            ofd->bgr24bytes = dvs->bgrframebufsize;
            ofd->jpeg = jpeg;
            ofd->jpegbytes = jpegbytes;
            ofd->width = pFrameInfo->nWidth;
            ofd->height = pFrameInfo->nHeight;
            ofd->sequence = ++dvs->sequence;
            ofd->timestamp = (double)pFrameInfo->nStamp;
            dvs->frameDataQueue.add(ofd);

            // FILE* fd{nullptr};
            // fd = fopen("/mnt/build/test.jpg", "wb+");
            // fwrite(ofd->jpeg, ofd->jpegbytes, 1, fd);
            // fclose(fd);

            // fd = fopen("/mnt/build/out.bgr", "wb+");
            // fwrite(ofd->bgr24, ofd->bgr24bytes, 1, fd);
            // fclose(fd);
        }
    }
    else if (T_AUDIO16 == pFrameInfo->nType)
    {
    }
}

void Dvs::getOneFrameData(OutputFrameData*& data)
{
    data = reinterpret_cast<OutputFrameData*>(frameDataQueue.front());
    frameDataQueue.remove();
}
