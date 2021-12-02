//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : 视频解码过滤器类
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_MEDIA_VIDEO_DECODER_FILTER_H
#define MODULE_MEDIA_VIDEO_DECODER_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class VideoDecoderFilter : public Filter
			{
			public:
				VideoDecoderFilter(void);
				virtual ~VideoDecoderFilter(void);

			public:
				int createNew(void* param = nullptr) override;
			};//class VideoDecoderFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_VIDEO_DECODER_FILTER_H
