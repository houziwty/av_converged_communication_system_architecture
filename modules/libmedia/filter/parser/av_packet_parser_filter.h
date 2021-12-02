//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV数据包解析过滤器
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_AV_PACKET_PARSER_FILTER_H
#define MODULE_MEDIA_AV_PACKET_PARSER_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class AVPacketParserFilter : public Filter
			{
			public:
				AVPacketParserFilter(void);
				virtual ~AVPacketParserFilter(void);

			public:
				int input(void* data = nullptr) override;
			};//class AVPacketParserFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_PACKET_PARSER_FILTER_H