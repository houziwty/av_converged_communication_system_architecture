//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-18
//		Description : AV编解码器
//
//		History:
//					1. 2022-02-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_CODEC_H
#define MODULE_AV_STREAM_AV_CODEC_H

#include "boost/function.hpp"
#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPkt;

			//编解码数据回调
			//@_1 [out] : 编解码ID
			//@_2 [out] : 数据
			typedef boost::function<void(const uint32_t, const AVPkt*)> AVCodecDataCallback;

			class AVcodec
			{
			public:
				AVcodec(
					AVCodecDataCallback callback, 
					const uint32_t id = 0);
				virtual ~AVcodec(void);

			public:
				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				virtual int input(const AVPkt* avpkt = nullptr) = 0;

			protected:
				const uint32_t cid;
				AVCodecDataCallback avcodecDataCallback;
			};//class AVcodec
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_CODEC_H
