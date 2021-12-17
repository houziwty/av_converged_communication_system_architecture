#include "http_flv_stream_session.h"

HttpFlvStreamSession::HttpFlvStreamSession(SessionPtr ptr) 
    : HttpSession(ptr)
{}

HttpFlvStreamSession::~HttpFlvStreamSession()
{}

void HttpFlvStreamSession::fetchSentDataEventNotification(
    const int e/* = 0*/, 
    const int bytes/* = 0*/)
{
}

void HttpFlvStreamSession::fetchReceivedDataEventNotification(
    const int e/* = 0*/, 
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    const std::string str((const char*)data, bytes);
    printf("[ %s ].\r\n", str.c_str());
}
