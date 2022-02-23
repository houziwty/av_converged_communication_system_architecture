//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : AV播放器
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PLAYER_H
#define MODULE_AV_STREAM_AV_PLAYER_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPkt;

			class AVPlayer
			{
			public:
				AVPlayer(const uint32_t id = 0);
				virtual ~AVPlayer(void);

			public:
				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				virtual int input(const AVPkt* avpkt = nullptr) = 0;

			protected:
				const uint32_t cid;
			};//class AVPlayer
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PLAYER_H
