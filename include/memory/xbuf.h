//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-16
//		Description : 数据缓存
//
//		History:
//					1. 05-16 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MEMORY_XBUF_H
#define FRAMEWORK_UTILS_MEMORY_XBUF_H

#include <stdint.h>
#include <string>
#include "boost/noncopyable.hpp"

namespace framework
{
	namespace utils
	{
		namespace memory
		{
            class XBuffer : public boost::noncopyable
            {
            public:
                //@bytes : 缓存容量大小
                XBuffer(void);
                virtual ~XBuffer(void);

            public:
                //输入数据
                //@_data : 数据
                //@_bytes : 大小
                //@Return ：错误码
                int input(const char* data = nullptr, const uint64_t bytes = 0);

                //获取数据
                //@Return : 数据
                inline const char* data(void) const
                {
                    return buf;
                }

                //获取大小
                //@Return : 大小
                inline const uint64_t bytes(void) const
                {
                    return buf_bytes;
                }

                //设置缓存
                //@_bytes : 大小
                //@Return : 数据
                int capacity(const uint64_t bytes = 0);

                //获取缓存容量大小
                inline const uint64_t capacity(void) const 
                {
                    return capacity_bytes;
                }

                //重置
                void reset(void);

            protected:
                char* buf;
                uint64_t buf_bytes;
                uint64_t capacity_bytes;
            };//class XBuffer

            class StringBuffer : public XBuffer
            {
            public:
                explicit StringBuffer(void);
                explicit StringBuffer(std::string str);
                explicit StringBuffer(const char* str = nullptr);
                StringBuffer(StringBuffer&& sb);
                StringBuffer(const StringBuffer& sb);
                virtual ~StringBuffer(void);

            public:
                StringBuffer& operator=(std::string str);
                StringBuffer& operator=(const char* str);
                StringBuffer& operator=(StringBuffer&& sb);
                StringBuffer& operator=(const StringBuffer& sb);

                inline char* data() const
                {
                    return const_cast<char*>(stringbuf.data()) + head;
                }

                inline const std::size_t size() const
                {
                    return stringbuf.size() - tail - head;
                }

                //删除
                StringBuffer& erase(const size_t pos = 0, const size_t n = std::string::npos);
                //追加
                StringBuffer& append(const StringBuffer& str);
                StringBuffer& append(const std::string& str);
                StringBuffer& append(const char* data = nullptr);
                StringBuffer& append(const char* data = nullptr, const std::size_t len = 0);
                //添加字符
                void push_back(const char c);
                //指定位置插入
                StringBuffer& insert(const std::size_t pos = 0, const char* str = nullptr, const std::size_t n = 0);
                //分配
                StringBuffer& assign(const char* str = nullptr);
                StringBuffer& assign(const char* str = nullptr, const std::size_t len = 0);

                //重置
                void reset(void);

                //获取字符
                char& operator[](const std::size_t pos);
                const char& operator[](const std::size_t pos) const;

                inline const std::size_t capacity() const
                {
                    return stringbuf.capacity();
                }

                void reserve(const std::size_t n = std::string::npos);

                void resize(const std::size_t n = std::string::npos, const char c = 0);

                inline const bool empty(void) const
                {
                    return size() <= 0;
                }

                const std::string substr(size_t pos, size_t n = std::string::npos) const;

            private:
                std::size_t head;
                std::size_t tail;
                std::string stringbuf;
            };//class StringBuffer
        }//namespace memory
    }//namespace utils
}//namespace framework

#endif //FRAMEWORK_UTILS_MEMORY_XBUF_H
