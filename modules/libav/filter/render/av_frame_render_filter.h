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

#ifndef MODULE_AV_STREAM_FRAME_RENDER_FILTER_H
#define MODULE_AV_STREAM_FRAME_RENDER_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFrameRenderFilter : public AVFilter
			{
			public:
				AVFrameRenderFilter(void);
				virtual ~AVFrameRenderFilter(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;
			};//class AVFrameRenderFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FRAME_RENDER_FILTER_H