#include "error_code.h"
#include "http_host_server.h"

HttpHostServer::HttpHostServer()
    : ASIONode(), HttpSessionNode(), sid{0}
{}

HttpHostServer::~HttpHostServer()
{}

uint32_t HttpHostServer::afterFetchAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        WriteLock wl{mtx};
        ++sid;
    }

    if (!e)
    {
        HttpSessionNode::add(sid);
    }

    return sid;
}

uint32_t HttpHostServer::afterFetchConnectedEventNotification(const int32_t e/* = 0*/)
{
    return 0;
}

void HttpHostServer::afterPolledReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        HttpSessionNode::request(id, data, bytes);
    }
    else
    {
        HttpSessionNode::remove(id);
    }
}

void HttpHostServer::afterPolledSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

int HttpHostServer::onresponse(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    ASIONode::send(id, data, bytes);
}
