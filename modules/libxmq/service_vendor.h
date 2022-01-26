//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-24
//		Description : XMQ服务节点角色
//
//		History:
//					1. 2022-01-24 由王科威创建
//

#ifndef MODULE_NETWORK_XMQ_SERVICE_VENDOR_H
#define MODULE_NETWORK_XMQ_SERVICE_VENDOR_H

#include "xmq_role.h"
#include "utils/url/url.h"
using namespace framework::utils::url;

namespace module
{
	namespace network
	{
		namespace xmq
		{
			typedef struct tagServiceInfo_t
			{
				char name[128];
			}ServiceInfo;

			typedef boost::function<void(const bool)> CheckOnlineStatusCallback;
			typedef boost::function<void(const ServiceInfo*, const uint32_t)> ServiceCapabilitiesNotificationCallback;

			class ServiceVendor : public XMQRole
			{
			public:
				ServiceVendor(
					const XMQModeConf& conf, 
					PolledDataCallback pollcb, 
					CheckOnlineStatusCallback checkcb, 
					ServiceCapabilitiesNotificationCallback capabilitiescb);
				~ServiceVendor(void);

			public:
				int run(ctx_t c = nullptr) override;
				int stop(void) override;
				//@Comment : 发送数据不需要服务名称
				int send(
					const char* /*name*/ = nullptr, 
					const void* data = nullptr, 
					const uint64_t bytes = 0) override;

			protected:
				void pollDataThread(void) override;
				void checkServiceOnlineStatusThread(void) override;

			private:
				//注册应答处理
				//@url [in] : 应答URL
				void processRegisterResponseMessage(Url& url);

				//查询应答处理
				//@url [in] : 应答URL
				void processQueryResponseMessage(Url& url);

			private:
				uint64_t registerResponseTimetamp;
				bool online;
				CheckOnlineStatusCallback checkOnlineStatusCallback;
				ServiceCapabilitiesNotificationCallback serviceCapabilitiesNotificationCallback;
			};//class ServiceVendor
		}//namespace xmq
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_XMQ_SERVICE_VENDOR_H
