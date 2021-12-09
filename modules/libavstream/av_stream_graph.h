//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : AV流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_GRAPH_H
#define MODULE_AV_STREAM_GRAPH_H

#include "defs.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVGraph
		{
		public:
			AVGraph(void);
			virtual ~AVGraph(void);

		public:
			//查询
			//@name : 名称
			//@Return : 过滤器实例
			AVFilterRef query(const std::string name);

			//创建
			//@param : 参数
			//@Return : 错误码
			//@Comment : 派生类必须先调用，再调用基类该方法，
			//					   否则可能导致派生类针脚连接异常
			virtual int createNew(void* param = nullptr);

			//销毁
			//@Return : 错误码
			virtual int destroy(void);

		protected:
			//串联针脚
			//@Return : 错误码
			//@Comment : 派生类必须实现自定义针脚连接
			//					   该方法必须由内部调用
			virtual int connectPin(void) = 0;

		protected:
			AVFilterPtrs avfilters;
		};//class AVGraph
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_GRAPH_H
