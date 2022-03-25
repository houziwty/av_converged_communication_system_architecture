//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-23
//		Description : JSON封装解析
//
//		History:
//					1. 2022-03-23 由王科威创建
//

#ifndef FRAMEWORK_UTILS_DATA_JSON_H
#define FRAMEWORK_UTILS_DATA_JSON_H

#include <vector>
#include "boost/property_tree/ptree.hpp"

namespace framework
{
    namespace utils
    {
        namespace data
        {
            using Item = std::pair<const std::string, const std::string>;
            using Items = std::vector<Item>;

            class Json
            {
            public:
                Json(void);
                virtual ~Json(void);

            public:
                //解析
                //@data [in] : 数据
                //@Return : 错误码
                int parse(const char* data = nullptr);

                //单节点添加
                //@items [in] : 值项
                //@Return : 错误码
                int add(
                    const std::string& path, const std::string& value);

				//数组节点添加
                //@name [in] : 子节点名称
				//@items [in] : 值项
				//@Return : 错误码
				int add(
                    const std::string& name,
					const Items& items);

            private:
                boost::property_tree::ptree root;
            };//class Json
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_DATA_JSON_H