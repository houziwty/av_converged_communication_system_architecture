#include "boost/make_shared.hpp"
#include "error_code.h"
// #include "session.h"
// using namespace module::network::asio;
#include "http_flv_stream_session.h"
#include "http_flv_stream_server.h"

HttpFlvStreamServer::HttpFlvStreamServer() 
//    : HttpServer()
{}

HttpFlvStreamServer::~HttpFlvStreamServer()
{}

// void HttpFlvStreamServer::fetchAcceptedEventNotification(SessionPtr session, const int e/* = 0*/)
// {
//     if(session && !e)
//     {
//         const std::string remoteAddr{
//             session->sock().remote_endpoint().address().to_string()};
//         printf("Fetch accepted event from remote = [ %s ].\r\n", remoteAddr.c_str());
// 
//         //const std::string sid{Uuid().createNew()};
//         using TcpSessionPtr = boost::shared_ptr<TcpSession>;
//         static TcpSessionPtr testPtr;
//         TcpSessionPtr ptr{boost::make_shared<HttpFlvStreamSession>(session)};
//          if(ptr && 
//             Error_Code_Success == ptr->createNew(session) &&
//             Error_Code_Success == ptr->receive())
//         {
// //            sessions.add("test", ptr);
//             testPtr.swap(ptr);
//         }
//     }
//}
