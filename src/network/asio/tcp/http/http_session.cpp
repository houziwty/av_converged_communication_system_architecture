#include "network/asio/tcp/http/http_session.h"
using namespace framework::network::asio;

HttpSession::HttpSession(IoSessionPtr sess) : TcpSession{sess}
{}

HttpSession::~HttpSession()
{}
