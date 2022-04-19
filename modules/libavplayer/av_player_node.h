//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : AV播放器节点
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PLAYER_NODE_H
#define MODULE_AV_STREAM_AV_PLAYER_NODE_H

#include "libavplayer_defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPlayerNode
			{
			public:
				AVPlayerNode(const uint32_t id = 0);
				virtual ~AVPlayerNode(void);

			public:
				//输入数据
				//@avpkt [in] : 数据
				//@Return : 错误码
				virtual int input(const void* avpkt = nullptr) = 0;

			protected:
				const uint32_t cid;
			};//class AVPlayerNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PLAYER_NODE_H
