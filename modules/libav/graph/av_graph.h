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

#ifndef MODULE_AV_STREAM_AV_GRAPH_H
#define MODULE_AV_STREAM_AV_GRAPH_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "map/unordered_map.h"
#include "libav_defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFilter;
			using AVFilterRef = boost::weak_ptr<AVFilter>;
			using AVFilterPtr = boost::shared_ptr<AVFilter>;
			using AVFilterPtrs = UnorderedMap<const std::string, AVFilterPtr>;

			class AVGraph
			{
			public:
				AVGraph(void);
				virtual ~AVGraph(void);

			public:
				//查询
				//@name [in] : 名称
				//@Return : 过滤器实例
				AVFilterRef query(const char* name = nullptr);

				//创建
				//@conf [in] : 配置参数
				//@Return : 错误码
				//@Comment : 派生类必须先调用，再调用基类该方法，否则可能导致派生类针脚连接异常
				virtual int createNew(const AVModeConf& conf);

				//销毁
				//@id [in] : 模块ID标识
				//@Return : 错误码
				virtual int destroy(const uint32_t id = 0);

			protected:
				//串联针脚
				//@Return : 错误码
				//@Comment : 派生类必须实现自定义针脚连接
				//					   该方法必须由内部调用
				virtual int connectPin(void) = 0;

			protected:
				AVFilterPtrs avfilters;
			};//class AVGraph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_GRAPH_H
