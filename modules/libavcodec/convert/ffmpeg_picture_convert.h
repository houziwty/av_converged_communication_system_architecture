//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : FFmpeg数据格式转换
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_FFMPEG_PICTURE_CONVERT_H
#define MODULE_AV_STREAM_FFMPEG_PICTURE_CONVERT_H

#include "av_codec.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class FFmpegPictureConvert : public AVcodec
            {
            public:
                FFmpegPictureConvert(
                    AVCodecDataCallback callback, 
					const uint32_t id = 0);
                virtual ~FFmpegPictureConvert(void);

            public:
                int input(const AVPkt* avpkt = nullptr) override;

            private:
                int init(const AVPkt* avpkt = nullptr);
                int uninit(void);

            private:
                void* cvt;
				void* iframe;
				void* oframe;
				uint8_t* obuffer;
                uint32_t obytes;
                uint32_t width;
                uint32_t height;
            };//class FFmpegPictureConvert
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FFMPEG_PICTURE_CONVERT_H
