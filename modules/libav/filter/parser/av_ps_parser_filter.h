//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV流PS数据包解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PS_PARSER_FILTER_H
#define MODULE_AV_STREAM_AV_PS_PARSER_FILTER_H

#include "av_parser_node.h"
#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPSParserFilter 
				: public AVFilter, protected AVParserNode
			{
			public:
				AVPSParserFilter(
					const AVFilterType type = AVFilterType::AV_FILTER_TYPE_NONE);
				virtual ~AVPSParserFilter(void);

			public:
				int createNew(const AVModeConf& conf) override;
				int destroy(void) override;
				int input(const AVPkt* avpkt = nullptr) override;

			protected:
				void afterParsedDataNotification(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) override;
			};//class AVPSParserFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PS_PARSER_FILTER_H