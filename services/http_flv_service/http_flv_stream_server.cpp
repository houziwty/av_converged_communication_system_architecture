#include "http_flv_stream_server.h"

HttpFlvStreamServer::HttpFlvStreamServer() : HttpServer()
{}

HttpFlvStreamServer::~HttpFlvStreamServer()
{}

void HttpFlvStreamServer::fetchAcceptedEventNotification(IoSessionPtr sessionPtr, const int e/* = 0*/)
{
    const std::string remoteAddr{
        sessionPtr->sock().remote_endpoint().address().to_string()};
    printf("Fetch accepted event from remote = [ %s ].\r\n", remoteAddr.c_str());

    HttpServer::fetchAcceptedEventNotification(sessionPtr, e);
}
