#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession(const std::string id) 
    : TcpSession(), uuid{id}
{}

HttpSession::~HttpSession()
{}

void HttpSession::fetchSentDataEventNotification(
    const int e/* = 0*/, 
    const int bytes/* = 0*/)
{
}

void HttpSession::fetchReceivedDataEventNotification(
    const int e/* = 0*/, 
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    const std::string str((const char*)data, bytes);
    printf("[ %s ].\r\n", str.c_str());
}
