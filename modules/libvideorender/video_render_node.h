//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : 视频显示节点
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_VIDEO_RENDER_NODE_H
#define MODULE_AV_STREAM_VIDEO_RENDER_NODE_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPkt;

			class VIDEO_RENDER_EXPORT VideoRenderNode
			{
			public:
				VideoRenderNode(void);
				virtual ~VideoRenderNode(void);

			public:
				//添加视频显示
				//@conf [in] : 视频显示参数
				//@Return : 错误码
				int addConf(const VideoRenderModeConf& conf);

				//删除视频显示
				//@id [in] : 视频显示ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 视频显示ID
				//@avpkt [in] : 数据包
				//@Return : 错误码
				int input(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr);
			};//class VideoRenderNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_VIDEO_RENDER_NODE_H
