#include "boost/make_shared.hpp"
#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "http_host_server.h"

int main(int argc, char* argv[])
{
    CommandLineParser parser;
    parser.setOption("http_port", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string http_port{parser.getParameter("http_port")};

        boost::shared_ptr<ASIONode> node{
            boost::make_shared<HttpHostServer>()};
        if(node && Error_Code_Success == node->run())
        {
            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = atoi(http_port.c_str());
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            node->addConf(conf);
            getchar();
            node->stop();
        }
    }

    return 0;
}