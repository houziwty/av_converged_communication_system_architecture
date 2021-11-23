//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_MEDIA_AV_GRAPH_H
#define MODULE_MEDIA_AV_GRAPH_H

#include "defs.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class Graph
			{
			public:
				Graph(void);
				virtual ~Graph(void);

			public:
				//查询过滤器实例
				//@name : 名称
				//@Return : 过滤器实例
				FilterPtr query(const std::string name);

				//创建流程图
				//@Return : 错误码
				//@Comment : 派生类必须先实现自定义的createNew()，再调用基类createNew()
				//					   否则派生类connectPin()将先于createNew()调用，从而导致针脚连接异常
				virtual int createNew(void);

			protected:
				//串联针脚
				//@Return : 错误码
				//@Comment : 派生类必须实现自定义针脚连接
				//					   该方法必须由内部调用
				virtual int connectPin(void) = 0;

			protected:
				FilterPtrs filters;
			};//class Graph
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_GRAPH_H
