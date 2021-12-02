//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : 音频播放过滤器类
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_MEDIA_AUDIO_PLAY_FILTER_H
#define MODULE_MEDIA_AUDIO_PLAY_FILTER_H

#include "libmedia/filter/media_target_filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class AudioPlayFilter 
				: public MediaTargetFilter
			{
			public:
				AudioPlayFilter(void);
				virtual ~AudioPlayFilter(void);

			public:
				int createNew(void* param = nullptr) override;
			};//class AudioPlayFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AUDIO_PLAY_FILTER_H