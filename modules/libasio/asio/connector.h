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

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace module
{
	namespace network
	{
		namespace asio
		{
			class Service;

			//连接事件回调
			//@_1 : socket
			//@_2 : 错误码
			typedef boost::function<void(boost::asio::ip::tcp::socket*, const int32_t)> ConnectedRemoteEventCallback;

			class Connector 
				: public boost::enable_shared_from_this<Connector>
			{
			public:
				//@ios [in] : IO服务
				//@callback [in] : 连接回调
				Connector(
					Service& ios, 
					ConnectedRemoteEventCallback callback);
				~Connector(void);

			public:
				//连接
				//@ip : 远程IP
				//@port : 远程端口号
				//@Return : 错误码
				int connect(
					const char* ip = nullptr,
					const uint16_t port = 0);

			private:
				//连接回调
				//@e : 错误码
				//@s : socket
				void afterConntectedRemoteCallback(
					boost::system::error_code e, 
					boost::asio::ip::tcp::socket* s = nullptr);

			private:
				Service& service;
				ConnectedRemoteEventCallback connectedRemoteEventCallback;
			};//class Connector
		}//namespace asio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_ASIO_CONNECTOR_H
