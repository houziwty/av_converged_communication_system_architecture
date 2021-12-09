//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流格式化转换过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_CONVERTER_FILTER_H
#define MODULE_AV_STREAM_CONVERTER_FILTER_H

#include "libavstream/av_stream_filter.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVStreamConverterFilter 
			: public AVFilter
		{
		public:
			AVStreamConverterFilter(void);
			virtual ~AVStreamConverterFilter(void);

		public:
			int input(void* data = nullptr) override;
		};//class AVStreamConverterFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_CONVERTER_FILTER_H