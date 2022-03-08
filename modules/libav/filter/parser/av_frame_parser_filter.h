//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV帧解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_FRAME_PARSER_FILTER_H
#define MODULE_AV_STREAM_AV_FRAME_PARSER_FILTER_H

#include "av_parser_node.h"
#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFrameParserFilter 
				: public AVFilter, protected AVParserNode
			{
			public:
				AVFrameParserFilter(
					const AVFilterType type = AVFilterType::AV_FILTER_TYPE_NONE);
				virtual ~AVFrameParserFilter(void);

			public:
				int createNew(const AVModeConf& conf) override;
				int destroy(const uint32_t id = 0) override;
				int input(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) override;

			protected:
				void afterParsedDataNotification(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) override;

			private:
				//AVFrameParserFilter和AVBufferParserFilter都使用AVParserNode
				//AVFrameParserFilter的模块ID设置偏移量10000以免与AVBufferParserFilter的模块ID重复
				const uint32_t offset;
			};//class AVFrameParserFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_FRAME_PARSER_FILTER_H