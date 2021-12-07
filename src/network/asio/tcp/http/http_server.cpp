#include "network/asio/tcp/http/http_server.h"
using namespace framework::network::asio;

HttpServer::HttpServer() : TcpServer()
{}

HttpServer::~HttpServer()
{}

void HttpServer::fetchAcceptedEventNotification(IoSessionPtr sessionPtr, const int e/* = 0*/)
{

}
