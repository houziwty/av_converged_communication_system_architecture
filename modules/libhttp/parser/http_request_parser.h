//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-16
//		Description : HTTP请求行消息解析器
//
//		History:
//					1. 2022-05-16 由王科威创建
//					2. 2022-07-05 由王科威重构HTTP请求解析逻辑和实现
//

#ifndef MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H
#define MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H

#include <stdint.h>
#include <unordered_map>

namespace module
{
	namespace network
	{
		namespace http
		{
            class HttpSession;

            class HttpRequestParser
            {
            public:
                //@bytes [in] : 缓存大小
                HttpRequestParser(
                    HttpSession& http, 
                    const uint32_t bytes = 3 * 1024 * 1024);
                ~HttpRequestParser(void);

            public:
                //输入数据
				//@request [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
                int input(
                    const void* request = nullptr, 
                    const uint32_t bytes = 0);

            private:
                //解析数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : <0错误码,>0解析大小
                int parse(
                    const char* data = nullptr, 
                    const uint32_t bytes = 0);

            private:
                HttpSession& httpSession;
                char* buffer;
                uint32_t bufBytes;
                uint32_t pos;

                std::string method;
                std::string url;
                std::string protocol;
                std::string content;
                std::unordered_map<std::string, std::string> headers;
            };//class HttpRequestParser
		}//namespace http
	}//namespace network
}//namespace module

#endif //MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H
