//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : 视频显示器
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_VIDE_RENDER_H
#define MODULE_AV_STREAM_VIDE_RENDER_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPkt;

			class VideoRender
			{
			public:
				VideoRender(const uint32_t id = 0);
				virtual ~VideoRender(void);

			public:
				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				virtual int input(const AVPkt* avpkt = nullptr) = 0;

			protected:
				const uint32_t cid;
			};//class VideoRender
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_VIDE_RENDER_H
