//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-17
//		Description : HTTP/RTSP/SIP协议解析器
//
//		History:
//					1. 2022-05-17 由王科威创建
//

#ifndef FRAMEWORK_UTILS_URL_URL_PARSER_H
#define FRAMEWORK_UTILS_URL_URL_PARSER_H

#include <string>

namespace framework
{
    namespace utils
    {
        namespace url
        {
            class UrlParser
            {
            public:
                UrlParser(void);
                ~UrlParser(void);

            public:
                //解析
                //@data [in] : 数据
                //@Return : >0表示已解析数据大小,<=0表示错误码
                int parse(const char* data = nullptr);

            private:
                //行参数解析
                //@data [in] : 数据
                //@Return : >0表示已解析数据大小,<=0表示错误码
                int parseOneLine(const char* line = nullptr);

            private:
                std::string method;
                std::string api;
                std::string proto;
            };//class UrlParser
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_URL_URL_PARSER_H