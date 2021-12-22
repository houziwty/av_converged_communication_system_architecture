//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 异步交换-发布模型
//
//		History:
//					1. 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_XMQ_SWITCHER_PUB_MODEL_H
#define FRAMEWORK_NETWORK_XMQ_SWITCHER_PUB_MODEL_H

#include "boost/shared_ptr.hpp"
#include "switcher.h"
#include "publisher.h"
using namespace module::network::xmq;
using SwitcherPtr = boost::shared_ptr<Switcher>;
using PublisherPtr = boost::shared_ptr<Publisher>;

namespace framework
{
	namespace network
	{
		namespace xmq
		{
			class SwitcherPubModel
			{
			public:
				SwitcherPubModel(void);
				virtual ~SwitcherPubModel(void);

			public:
				//启动
				//@switcherPort [in] : 端口号
				//@publisherPort [in] : 端口号
				//@hwm [in] : 缓存大小
				//@Return : 错误码
				virtual int start(
					const unsigned short switcherPort = 0,
					const unsigned short publisherPort = 0,
					const int hwm = 10);

				//停止
				//@Return : 错误码
				virtual int stop(void);

				//交换端发送
				//@uid [in] : 用户ID标识
				//@data [in] : 数据
				//@Return : 错误码
				virtual int send(
					const std::string uid, 
					const std::string data);

				//订阅端发送
				//@data [in] : 数据
				//@Return : 错误码
				virtual int send(const std::string data);

			protected:
				//交换模型数据接收回调函数
				//@uid [out] : 用户ID标识
				//@data [out] : 数据
				virtual void afterSwitcherPolledDataHandler(const std::string uid, const std::string data) = 0;

			private:
				//交换模型数据读取线程
				void pollDataFromSwitcherThread(void);

			private:
				SwitcherPtr switcherPtr;
				PublisherPtr publisherPtr;
				void* thread;
				bool stopped;
			};//class SwitcherPubModel
		}//namespace xmq
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_XMQ_SWITCHER_PUB_MODEL_H
