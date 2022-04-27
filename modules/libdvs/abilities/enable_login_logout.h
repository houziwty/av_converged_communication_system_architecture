//
//		Copyright : @2022, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-13
//		Description : 登录/注销使能接口
//
//		History:
//					1. 2022-04-13 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_ENABLE_LOGIN_AND_LOGOUT_H
#define MODULE_DEVICE_DVS_ENABLE_LOGIN_AND_LOGOUT_H

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class EnableLoginAndLogout
			{
			public:
				EnableLoginAndLogout(void){}
				virtual ~EnableLoginAndLogout(void){}

			public:
				//登录
				//@ip [in] : IP
    			//@port [in] : 端口号
				//@user [in] : 用户名
    			//@passwd [in] : 密码
				//@Return : 用户ID，小于0表示错误码
				virtual int64_t login(
					const char* ip = nullptr, 
					const uint16_t port = 0, 
					const char* user = nullptr, 
					const char* passwd = nullptr) = 0;

				//注销
				//@uid [in] : 用户ID
				//@Return : 错误码
				virtual int logout(const int64_t uid = -1) = 0;
			};//class EnableLoginAndLogout
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_ENABLE_LOGIN_AND_LOGOUT_H
