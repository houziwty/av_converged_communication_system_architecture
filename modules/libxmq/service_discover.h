//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-21
//		Description : XMQ服务发现角色
//
//		History:
//					1. 2022-01-21 由王科威创建
//					1. 2022-01-24 由王科威修改，在内部实现服务节点注册、节点查询和在线检测
//

#ifndef MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
#define MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H

#include "xmq_role.h"
#include "utils/url/url.h"
using namespace framework::utils::url;

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class ServiceDiscover : public XMQRole
			{
			public:
				ServiceDiscover(
					const XMQModeConf& conf, 
					PolledDataCallback pollcb);
				~ServiceDiscover(void);

			public:
				int run(ctx_t c = nullptr) override;
				int stop(void) override;
				int send(
					const char* name = nullptr, 
					const void* data = nullptr, 
					const uint64_t bytes = 0) override;

			protected:
				void pollDataThread(void) override;
				void checkServiceOnlineStatusThread(void) override;

			private:
				//注册业务处理
				//@name [in] : 请求服务名称
				//@requestUrl [in] : 请求URL标识
				void processRegisterMessage(const std::string name, Url& requestUrl);
			};//class ServiceDiscover
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
