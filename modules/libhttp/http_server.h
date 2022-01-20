//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : HTTP服务端
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_HTTP_SERVER_H
#define MODULE_NETWORK_HTTP_HTTP_SERVER_H

#include "tcp/tcp_server.h"
#include "tcp/tcp_session.h"
using namespace module::network::asio;
using TcpSessionPtr = boost::shared_ptr<TcpSession>;
#include "utils/map/unordered_map.h"
#include "http_session.h"

namespace module
{
	namespace network
	{
		namespace http
		{
			class NETWORK_HTTP_EXPORT HttpServer 
				: public TcpServer
			{
			public:
				HttpServer(void);
				virtual ~HttpServer(void);

			protected:
				void fetchAcceptedEventNotification(
					boost::asio::ip::tcp::socket* so = nullptr, 
					const int e = 0) override;

			private:
				UnorderedMap<const std::string, TcpSessionPtr> sessions;
			};//class HttpServer
		}//namespace http
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_HTTP_HTTP_SERVER_H
