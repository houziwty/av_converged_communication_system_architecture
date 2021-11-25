//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : AV数据回调过滤器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_MEDIA_AV_DATA_CALLBACK_FILTER_H
#define MODULE_MEDIA_AV_DATA_CALLBACK_FILTER_H

#include "libmedia/filter/media_target_filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class AVDataCallbackFilter : public MediaTargetFilter
			{
			public:
				AVDataCallbackFilter(void);
				virtual ~AVDataCallbackFilter(void);

			public:
				int createNew(void) override;
				int input(FramePtr frame) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class AVDataCallbackFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_DATA_CALLBACK_FILTER_H