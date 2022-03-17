//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-16
//		Description : FFmpeg JPEG图片编码器
//
//		History:
//					1. 2022-03-16 由王科威创建
//

#ifndef MODULE_AV_STREAM_FFMPEG_JPEG_ENCODE_H
#define MODULE_AV_STREAM_FFMPEG_JPEG_ENCODE_H

#include "av_codec.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class FFmpegJPEGEncode : public AVcodec
            {
            public:
                FFmpegJPEGEncode(
                    AVCodecDataCallback callback, 
					const uint32_t id = 0);
                virtual ~FFmpegJPEGEncode(void);

            public:
                int input(const AVPkt* avpkt = nullptr) override;

            private:
                int init(const AVPkt* avpkt = nullptr);
                int uninit(void);

            private:
                void* ctx;
                void* codec;
                void* iframe;
            };//class FFmpegJPEGEncode
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FFMPEG_JPEG_ENCODE_H
