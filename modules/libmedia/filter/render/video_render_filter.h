//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : 视频渲染过滤器类
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_MEDIA_VIDEO_RENDER_FILTER_H
#define MODULE_MEDIA_VIDEO_RENDER_FILTER_H

#include "libmedia/filter/media_target_filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class VideoRenderFilter : public MediaTargetFilter
			{
			public:
				VideoRenderFilter(void);
				virtual ~VideoRenderFilter(void);

			public:
				int createNew(void) override;
				int input(FramePtr frame) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class VideoRenderFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_VIDEO_RENDER_FILTER_H