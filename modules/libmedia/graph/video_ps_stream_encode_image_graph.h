//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-30
//		Description : 视频PS流编码图片流程图类
//
//		History:
//					1. 2021-11-30 由王科威创建
//

#ifndef VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H
#define VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H

#include "libmedia/graph.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class VideoPSStreamEncodeImageGraph : public Graph
			{
			public:
				VideoPSStreamEncodeImageGraph(void);
				virtual ~VideoPSStreamEncodeImageGraph(void);

			public:
				//创建流程图
				//@Return : 错误码
				//@Comment : 实时音视频流程图的构建内容包括 ：
				//						1. Video PS Parser Filter
				//						2. Video ES Decoder Filter
				//						3. Image Encoder Filter
				int createNew(void) override;

			protected:
				int connectPin(void) override;
			};//class VideoPSStreamEncodeImageGraph
		}//namespace av
	}//namespace media
}//namespace module

#endif//VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H
