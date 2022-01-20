#include "boost/make_shared.hpp"
#include "http_server.h"
using namespace module::network::http;

int main(int argc, char* argv[])
{
    boost::shared_ptr<TcpServer> server{boost::make_shared<HttpServer>()};
    server->start(50927);
    getchar();
    server->stop();
    
    return 0;
}