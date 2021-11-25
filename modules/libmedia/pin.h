//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 针脚类
//							1.暂时只支持一个输出连接一个输入
//							2.针脚类逻辑简单，可以不需要再继承
//							3.各实例之间的关联都没有采用只能指针的方式处理
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_MEDIA_AV_PIN_H
#define MODULE_MEDIA_AV_PIN_H

#include <string>
#include "boost/shared_ptr.hpp"

namespace module
{
	namespace media
	{
		namespace av
		{
			//针脚传输模式
			//必选其一
			typedef enum class tagTransportMode_t : int
			{
				TRANSPORT_MODE_INPUT = 0,
				TRANSPORT_MODE_OUTPUT
			}TransportMode;

			static const std::string innerDataInputPinName = "innerDataInputPin";
			static const std::string innerDataOutputPinName = "innerDataOutputPin";

			class Filter;
			using FilterPtr = boost::shared_ptr<Filter>;
			class Frame;
			using FramePtr = boost::shared_ptr<Frame>;

			class Pin
			{
			public:
				//@parent [in] : 拥有者实例
				//@mode [in] : 传输模式
				Pin(Filter& parent,
					const TransportMode tm = TransportMode::TRANSPORT_MODE_INPUT);
				~Pin(void);

			public:
				//连接下一流程过滤器的输入针脚
				//@inpin [in] : 输入针脚
				//@Return : 错误码
				//@Comment : 只有传输模式为Output的针脚才能连接一下流程的过滤器输入针脚
				int next(PinPtr inpin);

				//输入数据
				//@frame : 帧
				//@Return : 错误码
				int input(FramePtr frame);

				//获取传输模式
				//@Return : 传输模式
				inline const TransportMode getTransportMode(void) const
				{
					return mode;
				}

			private:
				const TransportMode mode;
				Filter& owner;
				PinPtr nextInputPin;
			};//class Pin
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_PIN_H
