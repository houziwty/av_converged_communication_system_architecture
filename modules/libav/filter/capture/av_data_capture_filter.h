//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV数据捕获过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_DATA_CAPTURE_FILTER_H
#define MODULE_AV_STREAM_AV_DATA_CAPTURE_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVDataCaptureFilter : public AVFilter
			{
			public:
				AVDataCaptureFilter(void);
				virtual ~AVDataCaptureFilter(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;
			};//class AVDataCaptureFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_DATA_CAPTURE_FILTER_H