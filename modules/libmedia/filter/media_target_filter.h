//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 媒体目的过滤器类
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_MEDIA_TARGET_FILTER_H
#define MODULE_MEDIA_MEDIA_TARGET_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class MediaTargetFilter : public Filter
			{
			public:
				MediaTargetFilter(void);
				virtual ~MediaTargetFilter(void);

			public:
				int createNew(void* param = nullptr) override;
			};//class MediaTargetFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_MEDIA_TARGET_FILTER_H