//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV视频帧分析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_VIDEO_FRAME_ANALYSIS_FILTER_H
#define MODULE_AV_STREAM_VIDEO_FRAME_ANALYSIS_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class VideoFrameAnalysisFilter : public AVFilter
			{
			public:
				VideoFrameAnalysisFilter(void);
				virtual ~VideoFrameAnalysisFilter(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;
			};//class VideoFrameAnalysisFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_VIDEO_FRAME_ANALYSIS_FILTER_H