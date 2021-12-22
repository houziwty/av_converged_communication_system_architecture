//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-04
//		Description : HTTP-FLV服务端
//
//		History:
//					1. 2021-12-04 由王科威创建
//

#ifndef SERVICE_HTTP_FLV_STREAM_SERVER_H
#define SERVICE_HTTP_FLV_STREAM_SERVER_H

#include "tcp/http/http_server.h"
using namespace module::network::asio;

class HttpFlvStreamServer 
	: public HttpServer
{
public:
	HttpFlvStreamServer(void);
	virtual ~HttpFlvStreamServer(void);

protected:
	void fetchAcceptedEventNotification(SessionPtr session, const int e = 0) override;
};//class HttpFlvStreamServer

#endif//SERVICE_HTTP_FLV_STREAM_SERVER_H
