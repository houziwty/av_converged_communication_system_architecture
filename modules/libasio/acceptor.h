//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 异步监听器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_ACCEPTOR_H
#define MODULE_NETWORK_ASIO_ACCEPTOR_H

#include "session.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			using SessionPtr = boost::shared_ptr<Session>;

			//监听事件回调
			//@_1 : 会话
			//@_2 : 错误码
			typedef boost::function<void(SessionPtr, const int)> AcceptedRemoteConnectEventCallback;

			class NETWORK_ASIO_EXPORT Acceptor 
				: public boost::enable_shared_from_this<Acceptor>
			{
			public:
				//@ctx [in] : 上下文实例
				//@port [in] : 端口号
				Acceptor(
					boost::asio::io_context& ctx, 
					AcceptedRemoteConnectEventCallback callback,
					const unsigned short port = 0);
				~Acceptor(void);

			public:
				//监听
				//@ctx [in] : 上下文实例
				//@Return : 错误码
				int listen(boost::asio::io_context& ctx);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				AcceptedRemoteConnectEventCallback acceptedRemoteConnectCBFunc;
			};//class Acceptor
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_ACCEPTOR_H
