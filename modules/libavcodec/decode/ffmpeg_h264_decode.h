//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-18
//		Description : FFmpeg H264视频解码器
//
//		History:
//					1. 2022-02-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_FFMPEG_H264_DECODE_H
#define MODULE_AV_STREAM_FFMPEG_H264_DECODE_H

#include "av_codec.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class FFmpegH264Decode : public AVcodec
            {
            public:
                FFmpegH264Decode(
                    AVCodecDataCallback callback, 
					const uint32_t id = 0);
                virtual ~FFmpegH264Decode(void);

            public:
                int input(const AVPkt* avpkt = nullptr) override;

            private:
                int initDecode(void);
                int uninitDecode(void);

            private:
                void* ctx;
                void* codec;
                void* cvt;
                void* picture;
                void* bgr24Frame;
                uint8_t* bgr24;
<<<<<<< HEAD
                uint32_t bgr24Bytes;
=======
                int bgr24Bytes;
>>>>>>> 988daff8f78d5aa9a26ee4853f763da606d880b4
            };//class FFmpegH264Decode
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FFMPEG_H264_DECODE_H
