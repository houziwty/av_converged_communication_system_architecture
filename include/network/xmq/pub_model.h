//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步发布模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_XMQ_PUB_MODEL_H
#define FRAMEWORK_NETWORK_XMQ_PUB_MODEL_H

#include "boost/shared_ptr.hpp"
#include "publisher.h"
using namespace module::network::xmq;
using PublisherPtr = boost::shared_ptr<Publisher>;

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class PubModel
			{
			public:
				Pub(void);
				virtual ~Pub(void);

			public:
				//启动
				//@port [in] : 端口号
				//@hwm [in] : 缓存大小
				//@Return : 错误码
				virtual int start(
					const unsigned short port = 0, 
					const int hwm = 10);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//发送
				//@data [in] : 数据
				//@Return : 错误码
				virtual int send(const std::string data);

			private:
				PublisherPtr publisherPtr;
			};//class PubModel
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_PUB_MODEL_H
