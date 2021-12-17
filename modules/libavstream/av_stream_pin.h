//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 针脚类
//							1.暂时只支持一个输出连接一个输入
//							2.针脚类逻辑简单，可以不需要再继承
//							3.针脚与过滤器之间使用引用的方式关联
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_PIN_H
#define MODULE_AV_STREAM_PIN_H

#include "defs.h"

namespace module
{
	namespace avstream
	{
		//针脚类型
		//必选其一
		typedef enum class tagPinType_t : int
		{
			PIN_TYPE_INPUT = 0,
			PIN_TYPE_OUTPUT
		}PinType;

		class AVPin
		{
		public:
			//@filter [in] : 拥有者
			//@pt [in] : 类型
			AVPin(AVFilter& filter,
				const PinType pt = PinType::PIN_TYPE_INPUT);
			virtual ~AVPin(void);

		public:
			//连接下一步过滤器的输入针脚
			//@pin [in] : 针脚
			//@Return : 错误码
			//@Comment : 只能Output连接Input
			virtual int connect(AVPinRef pin);

			//输入数据
			//@data : 数据
			//@Return : 错误码
			virtual int input(void* data = nullptr);

			//获取传输模式
			//@Return : 传输模式
			inline const PinType getPinType(void) const
			{
				return pinType;
			}

		protected:
			const PinType pinType;
			AVFilter& owner;
			AVPinRef nextInputPin;
		};//class AVPin
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_PIN_H
