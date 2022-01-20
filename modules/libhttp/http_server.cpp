#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "http_session.h"
#include "http_server.h"
using namespace module::network::http;

HttpServer::HttpServer() : TcpServer()
{}

HttpServer::~HttpServer()
{}

void HttpServer::fetchAcceptedEventNotification(
    boost::asio::ip::tcp::socket* so/* = nullptr*/, const int e/* = 0*/)
{
    const std::string uuid{Uuid().createNew()};
    TcpSessionPtr ptr{boost::make_shared<HttpSession>(uuid)};

    // if(ptr && 
    //    Error_Code_Success == ptr->createNew(session) && 
    //    Error_Code_Success == ptr->receive())
    if(ptr)
    {
        sessions.add(uuid, ptr);
    }
}
