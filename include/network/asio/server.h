//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : 服务器
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_ASIO_SERVER_H
#define FRAMEWORK_NETWORK_ASIO_SERVER_H

#include "boost/noncopyable.hpp"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class Server 
				: private boost::noncopyable
			{
			public:
				Server(void);
				virtual ~Server(void);

			public:
				//启动
				//@port [in] : 监听端口号
				//@Return : 错误码
				virtual int start(const unsigned short port = 0) = 0;

				//停止
				//@Return : 错误码
				virtual int stop(void) = 0;
			};//class Server
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_SERVER_H
