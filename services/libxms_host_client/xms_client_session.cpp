#include "error_code.h"
#include "libxms_host_client.h"
#include "xms_client_session.h"

XmsClientSession::XmsClientSession(
    LibXmsHostClient& host, 
    SessionPtr ptr, 
    const std::string sid) 
    : TcpSession(ptr), libXmsHostClient{host}, sessionID{sid}
{}

XmsClientSession::~XmsClientSession()
{}

void XmsClientSession::fetchSentDataEventNotification(
    const int e/* = 0*/, 
    const int bytes/* = 0*/)
{
    libXmsHostClient.fetchXmsClientSessionSentDataEventNotification(sessionID, e, bytes);
}

void XmsClientSession::fetchReceivedDataEventNotification(
    const int e/* = 0*/, 
    const void* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    libXmsHostClient.fetchXmsClientSessionReceivedDataEventNotification(sessionID, e, data, bytes);
}
