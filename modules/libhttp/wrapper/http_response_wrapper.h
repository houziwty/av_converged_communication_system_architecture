//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-07-15
//		Description : HTTP应答封装
//
//		History:
//					1. 2022-07-15 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_HTTP_RESPONSE_WRAPPER_H
#define MODULE_NETWORK_HTTP_HTTP_RESPONSE_WRAPPER_H

#include <map>

namespace module
{
	namespace network
	{
		namespace http
		{
            class HttpSession;

            class HttpResponseWrapper
            {
            public:
                //@session [in] : HTTP会话实例
                HttpResponseWrapper(HttpSession& session);
                ~HttpResponseWrapper(void);

            public:
                //封装
				//@protocol [in] : 协议/版本
				//@code [in] : HTTP错误码
                //@headers [in,out] : 消息头
				//@content [in,out] : 消息体
                //@rep [out] : 应答
				//@Return : 错误码
                int encode(
                    const std::string& protocol, 
                    const int code, 
                    std::multimap<std::string, std::string>& headers, 
                    std::string& content, 
                    std::string& rep);

            private:
                HttpSession& httpSession;
            };//class HttpResponseWrapper
		}//namespace http
	}//namespace network
}//namespace module

#endif //MODULE_NETWORK_HTTP_HTTP_RESPONSE_WRAPPER_H
