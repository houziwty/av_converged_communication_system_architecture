//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : AV数据处理类
//							1.PS转ES
//							2.AV解码
//							3.AV格式转换
//							4.AV算法计算
//							5.图片编码
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_AV_PROCESSOR_H
#define MODULE_MEDIA_AV_PROCESSOR_H

namespace module
{
	namespace media
	{
		namespace av
		{
			class AVProcessor
			{
			public:
				AVProcessor(void);
				virtual ~AVProcessor(void);

			public:
				//创建
				//@param : 参数
				//@Return : 错误码
				virtual int createNew(void* param = nullptr) = 0;

				//创建
				//@Return : 错误码
				virtual int destroy(void) = 0;

				//输入数据
				//@frame : 帧
				//@Return : 错误码
				virtual int input(void* data = nullptr) const = 0;
			};//class AVProcessor
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_PROCESSOR_H
