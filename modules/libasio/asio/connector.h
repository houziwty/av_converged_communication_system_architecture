//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : 异步连接器
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_CONNECTOR_H
#define MODULE_NETWORK_ASIO_CONNECTOR_H

#include "session.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			using SessionPtr = boost::shared_ptr<Session>;

			//连接事件回调
			//@_1 : 会话
			//@_2 : 错误码
			typedef boost::function<void(SessionPtr, const int)> ConnectedRemoteEventCallback;

			class NETWORK_ASIO_EXPORT Connector 
				: public boost::enable_shared_from_this<Connector>
			{
			public:
				Connector(
					ConnectedRemoteEventCallback callback);
				~Connector(void);

			public:
				//连接
				//@ctx [in] : 上下文 
				//@ip : 远程IP
				//@port : 远程端口号
				//@Return : 错误码
				int connect(
					boost::asio::io_context& ctx, 
					const char* ip = nullptr,
					const unsigned short port = 0);

			private:
				ConnectedRemoteEventCallback connectedRemoteCBFunc;
			};//class Connector
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_CONNECTOR_H
