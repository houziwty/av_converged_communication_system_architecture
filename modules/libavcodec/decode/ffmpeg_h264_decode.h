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
                int init(const AVPkt* avpkt = nullptr);
                int uninit(void);

            private:
                void* ctx;
                void* codec;
                void* iframe;
                void* oframe;
                uint8_t* obuffer;
                uint32_t obytes;
            };//class FFmpegH264Decode
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FFMPEG_H264_DECODE_H
