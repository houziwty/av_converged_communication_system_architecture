//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV帧编码过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_FRAME_ENCODER_FILTER_H
#define MODULE_AV_STREAM_AV_FRAME_ENCODER_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFrameEncoderFilter : public AVFilter
			{
			public:
				AVFrameEncoderFilter(void);
				virtual ~AVFrameEncoderFilter(void);

			public:
				int input(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) override;
			};//class AVFrameEncoderFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_FRAME_ENCODER_FILTER_H