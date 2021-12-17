//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : AV流播放过滤器
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_AV_STREAM_RENDER_FILTER_H
#define MODULE_AV_STREAM_RENDER_FILTER_H

#include "libavstream/av_stream_filter.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVStreamRenderFilter 
			: public AVFilter
		{
		public:
			AVStreamRenderFilter(void);
			virtual ~AVStreamRenderFilter(void);

		public:
			int input(void* data = nullptr) override;
		};//class AVStreamRenderFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_RENDER_FILTER_H