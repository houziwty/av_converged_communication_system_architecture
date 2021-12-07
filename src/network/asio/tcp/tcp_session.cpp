#include "network/asio/tcp/tcp_session.h"
using namespace framework::network::asio;

TcpSession::TcpSession(IoSessionPtr sess) : Session{sess}
{}

TcpSession::~TcpSession()
{}
