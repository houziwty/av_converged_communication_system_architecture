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
                int initConvert(void);
                int uninitConvert(void);

            private:
                void* cvt;
                uint8_t* rgb24;
                int rgb24Bytes;
                void* in;
                void* out;
            };//class FFmpegPictureConvert
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FFMPEG_PICTURE_CONVERT_H
