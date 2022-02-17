//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流数据包解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PKT_PARSER_FILTER_H
#define MODULE_AV_STREAM_AV_PKT_PARSER_FILTER_H

#include "av_parser_node.h"
#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPktParserFilter 
				: public AVFilter, protected AVParserNode
			{
			public:
				AVPktParserFilter(
					const AVFilterType type = AVFilterType::AV_FILTER_TYPE_NONE);
				virtual ~AVPktParserFilter(void);

			public:
				int createNew(void* param = nullptr) override;
				int destroy(void) override;
				int input(const AVPkt* avpkt = nullptr) override;

			protected:
				void afterParsedDataNotification(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) override;
			};//class AVPktParserFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PKT_PARSER_FILTER_H