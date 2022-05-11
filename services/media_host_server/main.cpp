#include "boost/make_shared.hpp"
#include "error_code.h"
#include "media_host_server.h"

int main(int argc, char* argv[])
{
    int ret{Error_Code_Success};
    char* localhost{"0.0.0.0"};
    FileLog log;
    log.createNew(nullptr, false);
    boost::shared_ptr<Libasio> media{
        boost::make_shared<MediaHostServer>()};
    ASIOModeConf conf;

    if (media)
    {
        conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
        conf.tcp.ip = localhost;

        //HTTP over TCP
        conf.port = 8080;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动HTTP(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动HTTP(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //HTTPS over TCP
        conf.port = 8443;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动HTTPS(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动HTTPS(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //RTSP over TCP
        conf.port = 8554;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTSP(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTSP(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //RTSPS over TCP
        conf.port = 8322;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTSPS(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTSPS(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //RTMP over TCP
        conf.port = 1935;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTMP(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTMP(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //RTMPS over TCP
        conf.port = 19350;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTMPS(%u)服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTMPS(%u)服务失败，错误码=%d", conf.port, ret);
        }

        //RTP over TCP
        conf.port = 20000;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTP(%u) over TCP服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTP(%u) over TCP服务失败，错误码=%d", conf.port, ret);
        }

        //RTP over UDP
        conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_UDP;
        conf.udp.sid = 0xFF050301;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTP(%u) over UDP服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTP(%u) over UDP服务失败，错误码=%d", conf.port, ret);
        }

        //WEBRTC over UDP
        conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_UDP;
        conf.port = 8000;
        conf.udp.sid = 0xFF080200;
        ret = media->addConf(conf);
        if (Error_Code_Success == ret)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动WEBRTC(%u) over UDP服务成功", conf.port);
        }
        else
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动WEBRTC(%u) over UDP服务失败，错误码=%d", conf.port, ret);
        }

        getchar();
        media->removeConf(0xFF050301);
        media->removeConf(0xFF080200);
        media.reset();
    }

    log.destroy();    
    return 0;
}