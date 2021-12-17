#include "boost/make_shared.hpp"
#include "http_flv_stream_server.h"

int main(int argc, char* argv[])
{
    using TcpServerPtr = boost::shared_ptr<TcpServer>;
    TcpServerPtr server{boost::make_shared<HttpFlvStreamServer>()};
    server->start(50927);
    getchar();
    server->stop();
    
    return 0;
}