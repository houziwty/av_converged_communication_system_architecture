//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-18
//		Description : HTTP错误码和消息体类型
//
//		History:
//					1. 2022-05-18 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_HTTP_CONST_H
#define MODULE_NETWORK_HTTP_HTTP_CONST_H

namespace module
{
	namespace network
	{
		namespace http
		{
                  extern "C"
                  {
                        //获取HTTP错误码描述
                        //@status [in] : 错误码
                        //@Return ：错误码描述
                        const char* getHttpStatusMessage(const int status = 200);

                        //获取HTTP MIME描述
                        //@name [in] : 后缀名
                        //@Return ：MIME描述
                        const char* getHttpContentType(const char* name = nullptr);
                  }
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_CONST_H
