//#include <cstring>
#include "boost/thread.hpp"
#include <thread>
#include "utils/memory/xmemcpy.h"
using namespace framework::utils::memory;
#include "dvs.h"

int Dvs::deviceCount = 0;

Dvs::Dvs(const int did) 
    : deviceId{did}, userID{-1}, realplayID{-1}, playID{-1}, stopped{true}, 
    ctx{nullptr}, iframe{nullptr}, oframe{nullptr}, iframebuf{nullptr}, oframebuf{nullptr}, sequence{0}, 
    oframebufsize{0}
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
    tg.join_all();
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
        NET_DVR_NETCFG_V30 cfg;
        memset(&cfg, 0, sizeof(NET_DVR_NETCFG_V30));
        if (NET_DVR_GetDVRConfig(userID, NET_DVR_GET_NETCFG_V30, 0,&cfg, sizeof(NET_DVR_NETCFG_V30), &ret))
        {
            sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
                cfg.struEtherNet[0].byMACAddr[0],
                cfg.struEtherNet[0].byMACAddr[1],
                cfg.struEtherNet[0].byMACAddr[2],
                cfg.struEtherNet[0].byMACAddr[3],
                cfg.struEtherNet[0].byMACAddr[4],
                cfg.struEtherNet[0].byMACAddr[5]);
        }

        NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
        struPlayInfo.hPlayWnd = 0;
        struPlayInfo.lChannel = struDeviceInfo.byStartChan;
        struPlayInfo.byProtoType = 1;
        struPlayInfo.bBlocked = 1;
        realplayID = NET_DVR_RealPlay_V40(userID, &struPlayInfo, &Dvs::RealDataCallBack_V30, this);

        if (-1 < realplayID)
        {
            tg.create_thread(boost::bind(&Dvs::decodeThreadHandler, this));
        }
    }

//    printf("ip=%s, port=%d, name=%s, password=%s, user=%d, realplay=%d, serialNo=%s.\r\n", 
//        ip.c_str(), port, name.c_str(), password.c_str(), userID, realplayID, struDeviceInfo.sSerialNumber);

    return 0;
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
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
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
 //       if(dvs->decodeDataCallback)
        {
            if(!dvs->ctx)
            {
                dvs->ctx = sws_getContext(
                    pFrameInfo->nWidth, pFrameInfo->nHeight, AV_PIX_FMT_YUV420P, 
                    pFrameInfo->nWidth, pFrameInfo->nHeight, AV_PIX_FMT_BGR24,
                    SWS_BILINEAR, 0, 0, 0);
                int bgr24_bytes{ av_image_get_buffer_size(AV_PIX_FMT_BGR24, pFrameInfo->nWidth, pFrameInfo->nHeight, 1) };
                dvs->oframebuf = av_malloc(bgr24_bytes);
                int yuv420p_Bytes{ av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pFrameInfo->nWidth, pFrameInfo->nHeight, 1) };
                dvs->iframebuf = av_malloc(yuv420p_Bytes);
                dvs->oframebufsize = bgr24_bytes;
                av_image_fill_arrays(
                    dvs->oframe->data, dvs->oframe->linesize, (const uint8_t*)dvs->oframebuf,
                    AV_PIX_FMT_BGR24, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
            }

            av_image_fill_arrays(
                dvs->iframe->data, dvs->iframe->linesize, (const uint8_t*)pBuf,
                AV_PIX_FMT_YUV420P, pFrameInfo->nWidth, pFrameInfo->nHeight, 1);
            uint8_t* temp{dvs->iframe->data[1]};
                dvs->iframe->data[1] = dvs->iframe->data[2];
                dvs->iframe->data[2] = temp;
            sws_scale(
                dvs->ctx, (uint8_t const* const*)dvs->iframe->data, dvs->iframe->linesize, 0, 
                pFrameInfo->nHeight, dvs->oframe->data, dvs->oframe->linesize);

            // DEC_API_DECODE_DATA data{0};
            // data.ts = (double)pFrameInfo->nStamp;
            // data.frameData = (unsigned char*)dvs->oframebuf;
            // data.channelNo = 3;
            // data.frameWidth = pFrameInfo->nWidth;
            // data.frameHeight = pFrameInfo->nHeight;
            // dvs->decodeDataCallback(data);

            void* jpeg{nullptr};
            int jpegbytes{0};
            dvs->jpegEncoder.input(dvs->iframe->data, pFrameInfo->nWidth, pFrameInfo->nHeight, jpeg, jpegbytes);            
            OutputFrameData* ofd{new(std::nothrow) OutputFrameData};

            if(ofd)
            {
                XMemory().copy(dvs->oframebuf, ofd->bgr24, dvs->oframebufsize);
                ofd->bgr24bytes = dvs->oframebufsize;
                XMemory().copy(jpeg, ofd->jpeg, jpegbytes);
                ofd->jpegbytes = jpegbytes;
                ofd->width = pFrameInfo->nWidth;
                ofd->height = pFrameInfo->nHeight;
                ofd->sequence = ++dvs->sequence;
                ofd->timestamp = (double)pFrameInfo->nStamp;
                dvs->frameDataQueue.add(ofd);
            }
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
