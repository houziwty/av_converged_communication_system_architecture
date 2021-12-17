//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 过滤器类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_FILTER_H
#define MODULE_AV_STREAM_FILTER_H

#include "defs.h"

namespace module
{
	namespace avstream
	{
		//过滤器类型
		//必选其一
		typedef enum class tagFilterType_t : int
		{
			FILTER_TYPE_SOURCE = 0, 
			FILTER_TYPE_MEDIUM,
			FILTER_TYPE_TARGET
		}FilterType;

		class AV_STREAM_EXPORT AVFilter
		{
		public:
			//@ft [in] : 类型
			AVFilter(const FilterType ft = FilterType::FILTER_TYPE_MEDIUM);
			virtual ~AVFilter(void);

		public:
			//查询
			//@name : 名称
			//@Return : 针脚实例
			AVPinRef query(const std::string name);

			//创建
			//@param : 参数
			//@Return : 错误码
			virtual int createNew(void* param = nullptr);

			//销毁
			//@Return : 错误码
			virtual int destroy(void);

			//输入数据
			//@data : 数据
			//@Return : 错误码
			virtual int input(void* data = nullptr) = 0;

		protected:
			const FilterType filterType;
			AVPinPtrs avpins;
			void* avproc;
		};//class AVFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_FILTER_H
