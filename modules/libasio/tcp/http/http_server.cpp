#include "libasio/tcp/http/http_server.h"
using namespace module::network::asio;

HttpServer::HttpServer() : TcpServer()
{}

HttpServer::~HttpServer()
{}

void HttpServer::fetchAcceptedEventNotification(SessionPtr session, const int e/* = 0*/)
{

}
