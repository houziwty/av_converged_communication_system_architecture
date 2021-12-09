//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流包解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_PARSER_FILTER_H
#define MODULE_AV_STREAM_PARSER_FILTER_H

#include "libavstream/av_stream_filter.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVStreamParserFilter 
			: public AVFilter
		{
		public:
			AVStreamParserFilter(void);
			virtual ~AVStreamParserFilter(void);

		public:
			int input(void* data = nullptr) override;
		};//class AVStreamParserFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_PARSER_FILTER_H