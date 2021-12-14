//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-28
//		Description : URL封装解析
//
//		History:
//					1. 2021-11-28 由王科威创建
//

#ifndef FRAMEWORK_UTILS_URL_URL_H
#define FRAMEWORK_UTILS_URL_URL_H

#include <string>
#include <vector>

namespace framework
{
    namespace utils
    {
        namespace url
        {
            typedef struct tagParamItem_t
            {
                std::string key;
                std::string value;
            }ParamItem;

            class Url
            {
            public:
                Url(void);
                ~Url(void);

            public:
                //解析
                //@data : 数据
                //@Return : 错误码
                int parse(const std::string data);

                //封装
                //@Return : URL
                const std::string encode(void);

                //获取协议名称
                //@Return : 协议名称
                inline const std::string getProtocol(void) const
                {
                    return protocol;
                }

                //设置协议名称
                //@name : 协议名称
                //@Return ：错误码
                int setProtocol(const std::string name);

                //获取主机名
                //@Return : 主机名
                //@Comment : 主机名为IP:Port
                inline const std::string getHost(void) const
                {
                    return host;
                }

                //设置主机名
                //@host : 主机名称或[IP:Port]
                //@Return : 错误码
                int setHost(const std::string host);

                //获取路径
                //@Return : 路径
                inline const std::string getPath(void) const
                {
                    return path;
                }

                //设置路径
                //@path : 路径，格式为/var1/var2
                //@Return : 错误码
                int setPath(const std::string path);

                //获取参数集
                //@Return : 参数集
                inline const std::vector<ParamItem> getParameters(void) const
                {
                    return parameters;
                }

                //添加参数
                //@key : 键
                //@value : 值
                //@Return : 错误码
                int addParameter(const std::string key, const std::string value);

                //删除参数
                //@key : 键
                //@Return : 错误码
                int removeParameter(const std::string key);

            private:
                std::string protocol;
                std::string host;
                std::string path;
                std::vector<ParamItem> parameters;
            };//class Url
        }//namespace url
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_URL_URL_H