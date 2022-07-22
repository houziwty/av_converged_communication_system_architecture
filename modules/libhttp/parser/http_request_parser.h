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
                //@session [in] : HTTP会话实例
                HttpRequestParser(HttpSession& session);
                ~HttpRequestParser(void);

            public:
                //解析
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 解析数据大小
                const std::size_t parse(
                    const char* data = nullptr, 
                    const std::size_t bytes = 0);

            private:
                HttpSession& httpSession;
            };//class HttpRequestParser
		}//namespace http
	}//namespace network
}//namespace module

#endif //MODULE_NETWORK_HTTP_HTTP_REQUEST_PARSER_H
