//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-18
//		Description : AV编解码库
//
//		History:
//					1. 2022-02-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_LIB_AV_CODEC_H
#define MODULE_AV_STREAM_LIB_AV_CODEC_H

#include "libavcodec_defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AV_CODEC_EXPORT Libavcodec
			{
			public:
				Libavcodec(void);
				virtual ~Libavcodec(void);

			public:
				//添加编解码
				//@conf [in] : 编解码参数
				//@Return : 错误码
				int addConf(const AVCodecModeConf& conf);

				//删除编解码
				//@id [in] : 编解码ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 编解码ID
				//@avpkt [in] : 数据
				//@Return : 错误码
				int input(
					const uint32_t id = 0, 
					const void* avpkt = nullptr);
				
			protected:
				//编解码数据通知
				//@id [out] : 编解码ID
				//@avpkt [in] : 数据
				virtual void afterCodecDataNotification(
					const uint32_t id = 0, 
					const void* avpkt = nullptr) = 0;
			};//class Libavcodec
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_LIB_AV_CODEC_H
