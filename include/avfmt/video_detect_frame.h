//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 视频检测帧类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef FRAMEWORK_MEDIA_VIDEO_DETECT_FRAME_H
#define FRAMEWORK_MEDIA_VIDEO_DETECT_FRAME_H

#include <vector>
#include "frame.h"

namespace framework
{
	namespace media
	{
		//视频检测结果区域
		typedef struct tagDetectResultPosition_t
		{
			int x;
			int y;
			int w;
			int h;
		}DetectResultPosition;

		class VideoDetectFrame final : public Frame
		{
		public:
			//@mt [in] : 主类型
			//@st [in] : 子类型
			VideoDetectFrame(
				const MainType mt = MainType::MAIN_TYPE_NONE,
				const SubType st = SubType::SUB_TYPE_NONE);
			~VideoDetectFrame(void);

		public:
			//设置输出数据和大小
			//@data [in] : 数据
			//@bytes [in] : 大小
			//@Return : 错误码
			int setOutputData(
				void* data = nullptr,
				const unsigned long long bytes = 0);

			//添加检测结果
			//@pos [in] : 检测结果
			//@Return : 错误码
			int addDetectResultPosition(const DetectResultPosition pos);

			//获取检测结果
			//@idx [in] : 索引号
			//@pos [in/out] : 检测结果
			//@Return : 错误码
			int getDetectResultPosition(const int idx, DetectResultPosition& pos);

		private:
			std::vector<DetectResultPosition> detectResultPositions;
		};//class VideoDetectFrame
	}//namespace media
}//namespace framework

#endif//FRAMEWORK_MEDIA_VIDEO_DETECT_FRAME_H
