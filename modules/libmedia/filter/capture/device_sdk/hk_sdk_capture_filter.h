//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 海康SDK数据获取过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_HK_SDK_CAPTURE_FILTER_H
#define MODULE_MEDIA_HK_SDK_CAPTURE_FILTER_H

#include "libmedia/filter/media_source_filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class HKSdkCaptureFilter : public MediaSourceFilter
			{
			public:
				HKSdkCaptureFilter(void);
				virtual ~HKSdkCaptureFilter(void);

			public:
				int createNew(void) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class PictureEncoderFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_HK_SDK_CAPTURE_FILTER_H