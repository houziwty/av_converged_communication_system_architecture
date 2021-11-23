//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 图片编码过滤器类
//							输入源必须是YUV420P
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_PICTURE_ENCODER_FILTER_H
#define MODULE_MEDIA_PICTURE_ENCODER_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class PictureEncoderFilter : public Filter
			{
			public:
				PictureEncoderFilter(void);
				virtual ~PictureEncoderFilter(void);

			public:
				int createNew(void) override;
				int input(FramePtr frame) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class PictureEncoderFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_PICTURE_ENCODER_FILTER_H