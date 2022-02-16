//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : AV过滤器类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_FILTER_H
#define MODULE_AV_STREAM_AV_FILTER_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "utils/map/unordered_map.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			typedef enum class tagAVFilterType_t : int
			{
				AV_FILTER_TYPE_NONE = 0, 
				AV_FILTER_TYPE_SOURCE, 
				AV_FILTER_TYPE_MEDIUM,
				AV_FILTER_TYPE_TARGET
			}AVFilterType;

			typedef struct tagAVFilterConf_t
			{
				int flag;					//flag表示过滤器创建的音视频针脚
											//1表示只创建视频针脚
											//2表示只创建音频针脚
											//3表示同时创建音视频针脚
				void* hwnd;					//hwnd表示视频播放窗口
			}AVFilterConf;

			class AVPin;
			class AVPkt;

			class AVFilter
			{
				using AVPinRef = boost::weak_ptr<AVPin>;
				using AVPinPtr = boost::shared_ptr<AVPin>;
				using AVPinPtrs = UnorderedMap<const std::string, AVPinPtr>;

			public:
				//@type [in] : 过滤器类型
				AVFilter(const AVFilterType type = AVFilterType::AV_FILTER_TYPE_NONE);
				virtual ~AVFilter(void);

			public:
				//查询
				//@name [in] : 名称
				//@Return : 针脚实例
				AVPinRef query(const std::string name);

				//创建
				//@param [in] : 参数
				//@Return : 错误码
				virtual int createNew(void* param = nullptr);

				//销毁
				//@Return : 错误码
				virtual int destroy(void);

				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				virtual int input(
					const AVPkt* avpkt = nullptr) = 0;

			protected:
				const AVFilterType filterType;
				AVPinPtrs avpins;
			};//class AVFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_FILTER_H
