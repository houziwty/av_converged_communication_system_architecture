//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV帧格式化转换过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_FRAME_CONVERTER_FILTER_H
#define MODULE_AV_STREAM_AV_FRAME_CONVERTER_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFrameConverterFilter : public AVFilter
			{
			public:
				AVFrameConverterFilter(void);
				virtual ~AVFrameConverterFilter(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;
			};//class AVFrameConverterFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_FRAME_CONVERTER_FILTER_H