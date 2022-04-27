//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-28
//		Description : URL解析
//
//		History:
//					1. 2021-11-28 由王科威创建
//

#ifndef FRAMEWORK_UTILS_DATA_URL_H
#define FRAMEWORK_UTILS_DATA_URL_H

#include <string>
#include <vector>

namespace framework
{
    namespace utils
    {
        namespace data
        {
            typedef struct tagParameter_t
            {
                std::string key;
                std::string value;
            }Parameter;

            class Url
            {
            public:
                Url(void);
                virtual ~Url(void);

            public:
                //解析
                //@data [in] : 数据
                //@bytes [in] : 大小
                //@Return : 错误码
                int parse(const void* data = nullptr, const uint64_t bytes = 0);

                //获取协议名称
                //@Return : 协议名称
                inline const std::string proto(void) const
                {
                    return _proto;
                }

                //设置协议名称
                //@name [in] : 协议名称
                //@Return ：错误码
                inline void proto(const std::string name)
                {
                    _proto = name;
                }

                //获取主机名
                //@Return : 主机名
                //@Comment : 主机名为IP:Port
                inline const std::string host(void) const
                {
                    return _host;
                }

                //设置主机名
                //@name [in] : 主机名称或[IP:Port]
                //@Return : 错误码
                inline void host(const std::string name)
                {
                    _host = name;
                }

                //获取路径
                //@Return : 路径
                inline const std::string path(void) const
                {
                    return _path;
                }

                //设置路径
                //@path [in] : 路径，格式为/var1/var2
                //@Return : 错误码
                inline void path(const std::string name)
                {
                    _path = name;
                }

                //获取参数集
                //@Return : 参数集
                inline const std::vector<Parameter>& parameters(void) const
                {
                    return _params;
                }

            private:
                std::string _proto;
                std::string _host;
                std::string _path;
                std::vector<Parameter> _params;
            };//class Url
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_DATA_URL_H