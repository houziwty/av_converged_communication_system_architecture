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

#ifndef MODULE_NETWORK_ASIO_IO_LISTEN_H
#define MODULE_NETWORK_ASIO_IO_LISTEN_H

#include "boost/asio.hpp"
#include "boost/function.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "io_session.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			using IoSessionPtr = boost::shared_ptr<IoSession>;
			//监听事件回调
			//@_1 : socket实例
			//@_2 : 错误码
			typedef boost::function<void(IoSessionPtr, const int)> RemoteConnectedEventCallback;

			class IoAcceptor 
				: protected boost::noncopyable
			{
			public:
				//@ctx [in] : io实例
				//@port [in] : 端口号
				IoAcceptor(
					boost::asio::io_context& ctx, 
					RemoteConnectedEventCallback callback, 
					const unsigned short port = 0);
				~IoAcceptor(void);

			public:
				//监听
				//@ctx [in] : io实例
				//@Return : 错误码
				int listen(boost::asio::io_context& ctx);

			private:
				boost::asio::ip::tcp::acceptor acceptor;
				RemoteConnectedEventCallback remoteConnectedCBFunc;
			};//class IoAcceptor
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_IO_LISTEN_H
