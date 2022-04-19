//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : AV针脚类
//							1.暂时只支持一个输出连接一个输入
//							2.针脚类逻辑简单，可以不需要再继承
//							3.针脚与过滤器之间使用引用的方式关联
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PIN_H
#define MODULE_AV_STREAM_AV_PIN_H

#include "boost/weak_ptr.hpp"

namespace module
{
	namespace av
	{
		namespace stream
		{
			typedef enum class tagAVPinType_t : int
			{
				PIN_TYPE_NONE = 0, 
				PIN_TYPE_INPUT, 
				PIN_TYPE_OUTPUT
			}AVPinType;

			class AVFilter;

			class AVPin
			{
				using AVPinRef = boost::weak_ptr<AVPin>;

			public:
				//@filter [in] : 拥有者
				//@type [in] : 类型
				AVPin(
					AVFilter& filter,
					const AVPinType type = AVPinType::PIN_TYPE_NONE);
				virtual ~AVPin(void);

			public:
				//连接
				//@pin [in] : 针脚
				//@Return : 错误码
				//@Comment : 只能Output连接Input
				virtual int connect(AVPinRef pin);

				//输入数据
				//@id [in] : 模块ID标识
				//@avpkt [in] : 数据
				//@Return : 错误码
				virtual int input(
					const uint32_t id = 0,
					const void* avpkt = nullptr);

				//获取类型
				//@Return : 类型
				inline const AVPinType type(void) const
				{
					return pinType;
				}

			protected:
				const AVPinType pinType;
				AVFilter& avfilter;
				AVPinRef inputPin;
			};//class AVPin
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PIN_H
