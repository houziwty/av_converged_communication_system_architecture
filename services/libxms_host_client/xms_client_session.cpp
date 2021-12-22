#include "error_code.h"
#include "libxms_host_client.h"
#include "xms_client_session.h"

XmsClientSession::XmsClientSession(
    LibXmsHostClient& host, 
    const std::string sid) 
    : TcpSession(), libXmsHostClient{host}, sessionId{sid}
{}

XmsClientSession::~XmsClientSession()
{}

void XmsClientSession::fetchSentDataEventNotification(
    const int e/* = 0*/, 
    const int bytes/* = 0*/)
{
    libXmsHostClient.fetchXmsClientSessionSentDataEventNotification(sessionId, e, bytes);
}

void XmsClientSession::fetchReceivedDataEventNotification(
    const int e/* = 0*/, 
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    libXmsHostClient.fetchXmsClientSessionReceivedDataEventNotification(sessionId, e, data, bytes);
}
