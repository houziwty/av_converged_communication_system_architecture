//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-16
//		Description : HTTP请求解析
//
//		History:
//					1. 2022-05-16 由王科威创建
//					2. 2022-07-05 由王科威重构HTTP协议解析逻辑和实现
//

#ifndef MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H
#define MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H

#include <map>
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace http
		{
            //HTTP请求解析回调
            //@_1 [out] : 方法
			//@_2 [out] : URL
            //@_3 [out] : 版本
			//@_4 [out] : 消息头
			//@_5 [out] : 消息体
            typedef boost::function<void(const std::string&, const std::string&, const std::string&, const std::multimap<std::string, std::string>&, const std::string&)> AfterParsedHttpRequestCallback;

            class HttpRequestParser
            {
            public:
                HttpRequestParser(
                    AfterParsedHttpRequestCallback callback);
                ~HttpRequestParser(void);

            public:
                //解析
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 解析数据大小
                const std::size_t parse(
                    const void* data = nullptr, 
                    const std::size_t bytes = 0);

            private:
                AfterParsedHttpRequestCallback afterParsedHttpRequestCallback;
            };//class HttpRequestParser
		}//namespace http
	}//namespace network
}//namespace module

#endif //MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H
