//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-14
//		Description : AV库
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-02-14 由王科威创建
//

#ifndef MODULE_AV_STREAM_LIB_AV_H
#define MODULE_AV_STREAM_LIB_AV_H

#include "libav_defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AV_EXPORT Libav
			{
			public:
				Libav(void);
				virtual ~Libav(void);

			public:
				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const AVModeConf& conf);

				//删除角色
				//@id [in] : AV流程图ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : AV流程图ID
				//@avpkt [in] : 数据
				//@Return : 错误码
				virtual int input(
					const uint32_t id = 0, 
					const void* avpkt = nullptr);
			};//class Libav
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_LIB_AV_H
