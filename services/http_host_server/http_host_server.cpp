#include "error_code.h"
#include "http_host_server.h"

HttpHostServer::HttpHostServer()
    : ASIONode(), HttpNode(), sid{0}
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
        HttpNode::add(sid);
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
        HttpNode::request(id, e, data, bytes);
    }
    else
    {
        HttpNode::remove(id);
    }
}

void HttpHostServer::afterPolledSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

void HttpHostServer::afterPolledReadDataNotification(
    const uint32_t id/* = 0*/, 
    const char* method/* = nullptr*/, 
    const char* path/* = nullptr*/)
{
    HttpNode::response(id, 200, "OK");
}
