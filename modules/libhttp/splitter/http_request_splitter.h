//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-16
//		Description : HTTP请求行分割
//
//		History:
//					1. 05-16 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_HTTP_REQUEST_SPLITTER_H
#define MODULE_NETWORK_HTTP_HTTP_REQUEST_SPLITTER_H

#include "memory/xbuf.h"
using namespace framework::utils::memory;

namespace module
{
	namespace network
	{
		namespace http
		{
            class HttpRequestSplitter
            {
            public:
                HttpRequestSplitter(void);
                virtual ~HttpRequestSplitter(void);

            public:
                //输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
                virtual int input(
                    const char* data = nullptr, 
                    const std::size_t len = std::string::npos);

                //重置
                void reset(void);

                //获取大小
                //@Return ：大小
                inline const std::size_t size(void) const
                {
                    return buffer_bytes;
                }

                //获取数据
                //@Return ：数据
                inline const char* data(void) const
                {
                    return buffer.data();
                }

            protected:
                /**
                 * 收到请求头
                 * @param data 请求头数据
                 * @param len 请求头长度
                 *
                 * @return 请求头后的content长度,
                 *  <0 : 代表后面所有数据都是content，此时后面的content将分段通过onRecvContent函数回调出去
                 *  0 : 代表为后面数据还是请求头,
                 *  >0 : 代表后面数据为固定长度content,此时将缓存content并等到所有content接收完毕一次性通过onRecvContent函数回调出去
                 */
                virtual ssize_t onRecvHeader(const char *data,size_t len) = 0;

                /**
                 * 收到content分片或全部数据
                 * onRecvHeader函数返回>0,则为全部数据
                 * @param data content分片或全部数据
                 * @param len 数据长度
                 */
                virtual void onRecvContent(const char *data,size_t len) {};

                /**
                 * 判断数据中是否有包尾
                 * @param data 数据指针
                 * @param len 数据长度
                 * @return nullptr代表未找到包位，否则返回包尾指针
                 */
                virtual const char *onSearchPacketTail(const char *data, size_t len);

                /**
                 * 设置content len
                 */
                void setContentLen(ssize_t content_len);

            private:
                std::size_t content_bytes;
                std::size_t buffer_bytes;
                StringBuffer buffer;
                //最大缓存1M数据
                static const std::size_t kMaxCacheSize;
            };//class HttpRequestSplitter
		}//namespace http
	}//namespace network
}//namespace module

#endif //MODULE_NETWORK_HTTP_HTTP_REQUEST_SPLITTER_H
