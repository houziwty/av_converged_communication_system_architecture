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

#include "boost/asio.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			//监听事件回调
			//@_1 : socket
			//@_2 : 错误码
			typedef boost::function<void(boost::asio::ip::tcp::socket* s, const int32_t)> AcceptedRemoteConnectEventCallback;

			class Acceptor
			{
			public:
				//@ioc [in] : io实例
				//@callback [in] : 监听回调
				//@port [in] : 监听端口号
				Acceptor(
					boost::asio::io_context& ioc, 
					AcceptedRemoteConnectEventCallback callback,
					const uint16_t port = 0);
				~Acceptor(void);

			public:
				//监听
				//@ioc [in] : io实例
				//@Return : 错误码
				int listen(boost::asio::io_context& ioc);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				AcceptedRemoteConnectEventCallback acceptedRemoteConnectEventCallback;
			};//class Acceptor
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_ACCEPTOR_H
