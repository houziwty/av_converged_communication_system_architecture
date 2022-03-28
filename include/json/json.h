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

                //获取单节点数据
                //@path [in] : 路径
                //@out [out] : 序列化字符串
                //@Return : 错误码
                int get(
                    const std::string& path, 
                    std::string& out);

				//获取数组节点数据
				//@path [in] : 路径
				//@out [out] : 序列化字符串
                //@Return : 错误码
				int get(
					const std::string& path,
					std::vector<std::string>& out);

                //序列化
                //@out [out] : 序列化字符串
                //@Return : 错误码
                int serialize(std::string& out);

                //单节点添加
                //@path [in] : 路径
                //@value [in] : 值
                //@Return : 错误码
                int add(
                    const std::string& path, 
                    const std::string& value);

				//数组节点添加
                //@name [in] : 子节点名称
				//@items [in] : 值项
				//@Return : 错误码
                //@Commnet ：不支持数组嵌套
				int add(
                    const std::string& name,
					const Items& items);

                //节点删除
				//@name [in] : 子节点名称
				//@Return : 错误码
				//@Commnet ：不支持无子节点的节点删除
                int remove(const std::string& name);

                //重置
                inline void reset(void)
                {
                    root.clear();
                }

            private:
                boost::property_tree::ptree root;
            };//class Json
        }//namespace data
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_DATA_JSON_H