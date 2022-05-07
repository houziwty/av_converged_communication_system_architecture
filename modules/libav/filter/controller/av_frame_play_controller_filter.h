//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-07
//		Description : AV流播放控制过滤器
//
//		History:
//					1. 2022-05-07 由王科威创建
//

#ifndef MODULE_AV_STREAM_FRAME_PLAY_CONTROLLER_FILTER_H
#define MODULE_AV_STREAM_FRAME_PLAY_CONTROLLER_FILTER_H

#include "filter/parser/av_buffer_parser_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFramePlayControllerFilter 
				: public AVBufferParserFilter
			{
			public:
				AVFramePlayControllerFilter(void);
				virtual ~AVFramePlayControllerFilter(void);

			public:
				int input(
					const uint32_t id = 0, 
					const void* avpkt = nullptr) override;
				int control(
					const uint32_t id = 0,
					const PlayControlType type = PlayControlType::PLAY_CONTROL_TYPE_NONE,
					const int32_t param = 0) override;

			private:
				PlayControlType playControlType;
				int32_t param;
			};//class AVFramePlayControllerFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FRAME_PLAY_CONTROLLER_FILTER_H