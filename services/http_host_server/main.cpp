#include "boost/foreach.hpp"
#include "boost/make_shared.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include "error_code.h"
#include "file/file.h"
using namespace framework::utils::file;
#include "http_host_server.h"

int main(int argc, char* argv[])
{
    //创建日志
    FileLog log;
    log.createNew(nullptr, false);
    int ret{Error_Code_Success};
    char* localhost{"0.0.0.0"};
    //读取服务端口配置信息
    const std::string path{Path().path() + "/media_host_server.conf"};
    boost::property_tree::ptree root;
    //创建服务
    boost::shared_ptr<Libasio> media{
        boost::make_shared<MediaHostServer>(log)};

    if (media)
    {
        try
        {
            boost::property_tree::read_json(path, root);
            boost::shared_ptr<MediaHostServer> mediaHostServer{
                boost::dynamic_pointer_cast<MediaHostServer>(media)};

            BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, root.get_child("port"))
            {
                const std::string proto{v.first};
                const uint16_t port{v.second.get_value<uint16_t>()};

                //TCP服务
                if (!proto.compare("http") || !proto.compare("https") || !proto.compare("rtsp") || !proto.compare("rtsps") || 
                    !proto.compare("rtmp") || !proto.compare("rtmps") || !proto.compare("rtp"))
                {
                    ASIOModeConf conf;
                    conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
                    conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
                    conf.tcp.ip = localhost;
                    conf.port = port;

                    ret = media->addConf(conf);
                    if (Error_Code_Success == ret)
                    {
                        log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动[ %s:%u ] over TCP服务成功", proto.c_str(), conf.port);
                    }
                    else
                    {
                        log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动[ %s:%u ]服务失败，错误码=[ %d ]", proto.c_str(),conf.port, ret);
                    }
                }
                
                //UDP服务
                if (!proto.compare("webrtc") || !proto.compare("rtp"))
                {
                    ASIOModeConf conf;
                    conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_UDP;
                    conf.port = (0 == proto.compare("rtp") ? port + 1000 : port);
                    conf.udp.sid = (0 == proto.compare("rtp") ? 0xFF050301 : 0xFF080200);
                    
                    ret = media->addConf(conf);
                    if (Error_Code_Success == ret)
                    {
                        log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动[ %s:%u ] over UDP服务成功", proto.c_str(), conf.port);
                    }
                    else
                    {
                        log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动[ %s:%u ] over UDP服务失败，错误码=[ %d ]", proto.c_str(), conf.port, ret);
                    }
                }

                if (Error_Code_Success == ret)
                {
                    mediaHostServer->addPort(proto, port);
                }
            }

            mediaHostServer->loadApi();
        }
        catch(const std::exception& e)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "解析配置文件[ %s ]异常，错误描述=[ %s ]", 
                path.c_str(), e.what());
        }
    }
    else
    {
        log.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "创建流媒体发布实例失败.");
    }

    getchar();
    log.destroy();    
    return 0;
}