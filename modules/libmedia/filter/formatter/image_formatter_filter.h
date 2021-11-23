//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 图片格式化过滤器类
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_IMAGE_FORMATTER_FILTER_H
#define MODULE_MEDIA_IMAGE_FORMATTER_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class ImageFormatterFilter : public Filter
			{
			public:
				ImageFormatterFilter(void);
				virtual ~ImageFormatterFilter(void);

			public:
				int createNew(void) override;
				int input(FramePtr frame) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class ImageFormatterFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_IMAGE_FORMATTER_FILTER_H