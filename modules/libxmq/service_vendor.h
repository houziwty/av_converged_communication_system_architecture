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
				int send(
					const void* data = nullptr, 
					const uint64_t bytes = 0, 
					const char* /*id */= nullptr) override;

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

				//转发业务处理
				//@from [in] : 源ID
				//@to [in] : 目的ID
				//@data [in] : 源数据
				void processForwardCustomMessage(const std::string data);

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
