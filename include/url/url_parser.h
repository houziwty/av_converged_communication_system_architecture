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

#include <map>
#include <string>
#include <vector>

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

                //获取方法名
                //Return : 方法名
                inline const std::string& method(void) const
                {
                    return _method;
                }

                //获取URL
                //Return : URL
                inline const std::string& url(void) const
                {
                    return _url;
                }

                //获取协议
                //Return : 协议名称/版本
                inline const std::string& proto(void) const
                {
                    return _proto;
                }

                //获取消息头字段值
                //@key [in] : 名称
                //Return : 值集合
                const std::vector<std::string> value(const std::string& key);

            private:
                //行参数解析
                //@data [in] : 数据
                //@Return : >0表示已解析数据大小,<=0表示错误码
                int parseOneLine(const char* line = nullptr);

            private:
                std::string _method;
                std::string _url;
                std::string _proto;
                std::multimap<std::string, std::string> _headers;
            };//class UrlParser
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_URL_URL_PARSER_H