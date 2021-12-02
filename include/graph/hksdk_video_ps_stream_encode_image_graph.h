//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-30
//		Description : 海康设备SDK视频PS流编码图片流程图类
//
//		History:
//					1. 2021-11-30 由王科威创建
//

#ifndef HKSDK_VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H
#define HKSDK_VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H

#include "boost/function.hpp"
#include "libmedia/graph/video_ps_stream_encode_image_graph.h"
using namespace module::media::av;

namespace framework
{
	namespace media
	{
		namespace av
		{
			typedef boost::function<void(const int, void*)> EncodeImageDataCallback;

			class HKSdkVideoPSStreamEncodeImageGraph 
				: public VideoPSStreamEncodeImageGraph
			{
			public:
				HKSdkVideoPSStreamEncodeImageGraph(const int did, const std::string url, EncodeImageDataCallback callback);
				virtual ~HKSdkVideoPSStreamEncodeImageGraph(void);

			public:
				//创建流程图
				//@Return : 错误码
				//@Comment : 实时音视频流程图的构建内容包括 ：
				//						1. HK SDK Filter
				//						2. Data Output Filter
				int createNew(void) override;

			protected:
				int connectPin(void) override;
				void processDataCompleteNotification(void* data = nullptr) override;

			private:
				const int deviceId;
				const std::string streamUrl;
				EncodeImageDataCallback encodeImageDataCallback;
			};//class HKSdkVideoPSStreamEncodeImageGraph
		}//namespace av
	}//namespace media
}//namespace module

#endif//HKSDK_VIDEO_PS_STREAM_ENCODE_IMAGE_GRAPH_H
