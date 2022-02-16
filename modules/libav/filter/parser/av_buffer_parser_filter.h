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

#ifndef MODULE_AV_STREAM_AV_BUFFER_PARSER_FILTER_H
#define MODULE_AV_STREAM_AV_BUFFER_PARSER_FILTER_H

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
				int createNew(void* /*param*/ = nullptr) override;
				int destroy(void) override;
				int input(
					const AVPkt* avpkt = nullptr) override;

			protected:
				void afterParsedDataNotification(
					const uint32_t id = 0, 
					const MainType maintype = MainType::MAIN_TYPE_NONE, 
					const SubType subtype = SubType::SUB_TYPE_NONE, 
					const uint32_t bytes = 0, 
					const uint64_t seqence = 0, 
					const uint64_t timestamp = 0, 
					const void* data = nullptr);
			};//class AVDataParserFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_BUFFER_PARSER_FILTER_H