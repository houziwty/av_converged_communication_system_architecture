//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : AV帧类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_MEDIA_AV_AV_FRAME_H
#define MODULE_MEDIA_AV_AV_FRAME_H

namespace module
{
	namespace media
	{
		namespace av
		{
			typedef enum class tagMainType_t : int
			{
				MAIN_TYPE_NONE = 0,
				MAIN_TYPE_STANDARD_H264 = 1,
				MAIN_TYPE_STANDARD_H265 = 2,
				MAIN_TYPE_STANDARD_ES = 3,
				MAIN_TYPE_HIKVISION_PRIVATE = 4,
				MAIN_TYPE_HIKVISION_PS = 5,
				MAIN_TYPE_RAW_VIDEO = 6,
				MAIN_TYPE_RAW_AUDIO = 7
			}MainType;

			typedef enum class tagSubType_t : int
			{
				SUB_TYPE_NONE = 0,
				SUB_TYPE_HEADER_INFO = 1,
				SUB_TYPE_IDR = 2,
				SUB_TYPE_P_FRAME = 3,
				SUB_TYPE_B_FRAME = 4,
				SUB_TYPE_YV12_FRAME = 5,
				SUB_TYPE_YUV420P_FRAME = 6,
				SUB_TYPE_BGR24_FRAME = 7,
				SUB_TYPE_JPEG_FRAME = 8,
				SUB_TYPE_PNG_FRAME = 9,
				SUB_TYPE_AAC_FRAME = 10,
				SUB_TYPE_G711_FRAME = 11,
				SUB_TYPE_G722_FRAME = 12,
			}SubType;

			class AVFrame
			{
			public:
				//@mt [in] : 主类型
				//@st [in] : 子类型
				AVFrame(
					const MainType mt = MainType::MAIN_TYPE_NONE,
					const SubType st = SubType::SUB_TYPE_NONE);
				virtual ~AVFrame(void);

			public:
				//获取主/子类型
				//@Return : 主子类型
				inline const MainType getMainType(void) const
				{
					return mainType;
				}
				inline const SubType getSubType(void) const
				{
					return subType;
				}

				//获取数据
				//@Return : 数据
				inline void* getInputData(void) const
				{
					return in;
				}

				//获取数据大小
				//@Return : 数据大小
				inline const unsigned long long getInputBytes(void) const
				{
					return insize;
				}

				//输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int input(
					void* data = nullptr,
					const unsigned long long bytes = 0);
				
			private:
				const MainType mainType;
				const SubType subType;
				void* in;
				unsigned long long insize;
			};//class AVFrame
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_AV_FRAME_H