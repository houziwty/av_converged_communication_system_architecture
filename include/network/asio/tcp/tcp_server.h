//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : TCP服务端
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_TCP_SERVER_H
#define FRAMEWORK_NETWORK_ASIO_TCP_SERVER_H

#include "boost/shared_ptr.hpp"
#include "libasio/io_service.h"
#include "libasio/io_acceptor.h"
#include "libasio/io_session.h"
using namespace module::network::asio;
using IoServicePtr = boost::shared_ptr<IoService>;
using IoAcceptorPtr = boost::shared_ptr<IoAcceptor>;
#include "network/asio/server.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class TcpServer 
				: public Server
			{
			public:
				TcpServer(void);
				virtual ~TcpServer(void);

			public:
				int start(const unsigned short port = 0) override;
				int stop(void);

			protected:
				//监听事件通知
				//@sessionPtr [in] : 会话实例
				//@e [in] : socket错误码
				virtual void fetchAcceptedEventNotification(IoSessionPtr sessionPtr, const int e = 0) = 0;

			private:
				IoServicePtr ioServicePtr;
				IoAcceptorPtr ioAcceptorPtr;
			};//class TcpServer
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_TCP_SERVER_H
