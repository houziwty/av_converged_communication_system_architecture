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
//

#ifndef MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
#define MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H

#include "xmq_role.h"

namespace module
{
	namespace network
	{
		namespace xmq
		{
			class ServiceDiscover : public XMQRole
			{
			public:
				ServiceDiscover(const XMQModeConf& conf);
				~ServiceDiscover(void);

			public:
				int run(ctx_t c = nullptr) override;
				int stop(void) override;

			protected:
				void pollDataThread(void);

			private:
				//检测服务在线状态线程
				void checkServiceOnlineStatusThread(void);

			private:
				const XMQModeConf modeconf;
				socket_t so;
			};//class ServiceDiscover
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SERVICE_DISCOVER_H
