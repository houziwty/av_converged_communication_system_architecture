//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : TCP会话
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_TCP_SESSION_H
#define MODULE_NETWORK_ASIO_TCP_SESSION_H

#include "boost/enable_shared_from_this.hpp"
#include "../session/session.h"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class TcpSession 
				: public Session, 
				public boost::enable_shared_from_this<TcpSession>
			{
			public:
				TcpSession(
					void* s = nullptr, 
					const uint32_t id = 0);
				virtual ~TcpSession(void);

			public:				
				int destroy(void) override;
				int send(
					const void* data = nullptr,
					const uint64_t bytes = 0, 
					const char* /* ip */= nullptr, 
					const uint16_t/* port */= 0) override;
				int receive(void) override;
			};//class TcpSession
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_TCP_SESSION_H
