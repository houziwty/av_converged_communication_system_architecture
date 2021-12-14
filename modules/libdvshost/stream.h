//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : DVS流
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DVS_HOST_STREAM_H
#define MODULE_DVS_HOST_STREAM_H

#include "defs.h"

namespace module
{
	namespace dvs
	{
		class DVS_HOST_EXPORT Stream
		{
		public:
			Stream(void);
			virtual ~Stream(void);

		public:
			//获取流类型
			//@Return : 流类型
			virtual const StreamType type(void) const = 0;
			
			//打开
			//@userID [in] : 用户ID，由Device实例login方法创建
			//@index [in] : 通道索引号
			//@type [in] : 码流类型，范围[0, 10]
			//@Return : 流ID
			virtual int open(
				const int userID = -1, 
				const int index = -1, 
				const unsigned int type = 0) = 0;

			//关闭
			//@streamID [in] : 流ID
			//@Return : 错误码
			virtual int close(const int streamID = -1) = 0;
		};//class Stream
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_STREAM_H
