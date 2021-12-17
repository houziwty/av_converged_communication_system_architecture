#include "libasio/tcp/http/http_session.h"
using namespace module::network::asio;

HttpSession::HttpSession(SessionPtr ptr) : TcpSession{ptr}
{}

HttpSession::~HttpSession()
{}
