//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流编码过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_ENCODER_FILTER_H
#define MODULE_AV_STREAM_ENCODER_FILTER_H

#include "libavstream/av_stream_filter.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVStreamEncoderFilter 
			: public AVFilter
		{
		public:
			AVStreamEncoderFilter(void);
			virtual ~AVStreamEncoderFilter(void);

		public:
			int input(void* data = nullptr) override;
		};//class AVStreamEncoderFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_ENCODER_FILTER_H