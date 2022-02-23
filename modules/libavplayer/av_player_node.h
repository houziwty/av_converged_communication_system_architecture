//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : AV播放节点
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PLAYER_NODE_H
#define MODULE_AV_STREAM_AV_PLAYER_NODE_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPkt;

			class AV_PLAYER_EXPORT AVPlayerNode
			{
			public:
				AVPlayerNode(void);
				virtual ~AVPlayerNode(void);

			public:
				//添加播放器
				//@conf [in] : 播放器参数
				//@Return : 错误码
				int addConf(const AVPlayerModeConf& conf);

				//删除播放器
				//@id [in] : 播放器ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 播放器ID
				//@avpkt [in] : 数据包
				//@Return : 错误码
				int input(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr);
			};//class AVPlayerNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PLAYER_NODE_H
