//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流数据解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_DATA_PARSER_FILTER_H
#define MODULE_AV_STREAM_DATA_PARSER_FILTER_H

#include "av_parser_node.h"
#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVDataParserFilter 
				: public AVFilter, protected AVParserNode
			{
			public:
				AVDataParserFilter(void);
				virtual ~AVDataParserFilter(void);

			public:
				int input(void* data = nullptr) override;
			};//class AVDataParserFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_DATA_PARSER_FILTER_H