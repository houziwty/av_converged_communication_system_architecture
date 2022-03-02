#include <iostream>
#include "videostream.h"

using namespace std;

int main(int argc,char *argv[])
{  



    const std::string asio_addr{"192.168.2.172"};

    ASIOModeConf conf;
    conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
    conf.port = 60820;
    conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
    conf.tcp.ip = (char*)asio_addr.c_str();

    ASIONode *videoNode = new videoStream ();

    auto ret = videoNode->run();

    printf("run: %d\n",ret);
    ret =  videoNode->addConf(conf);
    printf("config: %d\n",ret);

    getchar();
    videoStream *  videostream =  dynamic_cast <videoStream*>(videoNode);
    if(videostream != nullptr)
    {
        videostream->openStream();
    }
    getchar();

}
