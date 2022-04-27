//
//		Copyright : @2022, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-13
//		Description : 异常捕获使能接口
//
//		History:
//					1. 2022-04-13 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_ENABLE_CATCH_EXCEPTION_H
#define MODULE_DEVICE_DVS_ENABLE_CATCH_EXCEPTION_H

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class EnableCatchException
			{
			public:
				EnableCatchException(void){}
				virtual ~EnableCatchException(void){}

			public:
				//捕获异常
				//@Return : 错误码
				virtual int catchException(void) = 0;
			};//class EnableCatchException
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_ENABLE_CATCH_EXCEPTION_H
