//#include <unordered_map.h>
#include <regex>
#include "boost/json.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "file/file.h"
using namespace framework::utils::file;
#include "media_host_server.h"

int main(int argc, char* argv[])
{
    int ret{Error_Code_Success};
    char* localhost{"0.0.0.0"};
    const std::string path{Path().path() + "/media_port.conf"};
    uint64_t confbytes{512 * 1024};
    char* confbuf{new char[confbytes]};
    memset(confbuf, 0, confbytes);
    FileLog log;
    log.createNew(nullptr, false);
    boost::shared_ptr<Libasio> media{
        boost::make_shared<MediaHostServer>(log)};
    ASIOModeConf conf;

    const char* test_json{"{\"http\":8080,\"https\":8443,\"rtsp\":8554,\"rtsps\":8322,\"rtmp\":1935,\"rtmps\":19350,\"rtp\":20000,\"webrtc\":8000}"};

    if (media)
    {
        try
        {
            File().read(path.c_str(), confbuf, confbytes);
            const std::string temp{boost::json::serialize(std::string(confbuf))};
            const std::string strRes{boost::regex_replace(temp, boost::regex(" |\n|\r|\t"), "")};
            log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "%s", strRes.c_str());

            std::string str ="sdfklsd l;sdfsdfsdf\nsdf    \ns\r\n\t\t\n  sddsf111{ sdf} f s    ";
 
	
            std::regex pattern(" |\n|\r|\t");
            std::string fmt = "";
        
            std::string retregex = std::regex_replace(confbuf, pattern, fmt);

            auto jo{boost::json::parse(temp).as_object()};

            //TCP服务
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            conf.tcp.ip = localhost;
            
            //HTTP over TCP
            if(jo.contains("http"))
            {
                conf.port = static_cast<uint16_t>(jo["http"].as_uint64());
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动HTTP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动HTTP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //HTTPS over TCP
            if(jo.contains("https"))
            {
                conf.port = 8443;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动HTTPS[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动HTTPS[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //RTSP over TCP
            if(jo.contains("rtsp"))
            {
                conf.port = 8554;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTSP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTSP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //RTSPS over TCP
            if (jo.contains("rtsps"))
            {
                conf.port = 8322;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTSPS[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTSPS[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //RTMP over TCP
            if (jo.contains("rtmp"))
            {
                conf.port = 1935;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTMP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTMP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //RTMPS over TCP
            if (jo.contains("rtmps"))
            {
                conf.port = 19350;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTMPS[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTMPS[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            if (jo.contains("rtp"))
            {
                //RTP over TCP
                conf.port = 20000;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTP over TCP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTP over TCP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }

            //UDP服务
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_UDP;

            if (jo.contains("rtp"))
            {
                //RTP over UDP
                conf.udp.sid = 0xFF050301;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动RTP over UDP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动RTP over UDP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }
            
            if (jo.contains("webrtc"))
            {
                //WEBRTC over UDP
                conf.port = 8000;
                conf.udp.sid = 0xFF080200;
                ret = media->addConf(conf);
                if (Error_Code_Success == ret)
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_INFO, "启动WEBRTC over UDP[%u]服务成功", conf.port);
                }
                else
                {
                    log.write(SeverityLevel::SEVERITY_LEVEL_WARNING, "启动WEBRTC over UDP[%u]服务失败，错误码=[%d]", conf.port, ret);
                }
            }
        }
        catch(const std::exception& e)
        {
            log.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "解析media_port.conf配置[%s]异常，错误描述=[%s]", test_json, e.what());
        }

        getchar();
        media->removeConf(0xFF050301);
        media->removeConf(0xFF080200);
        media.reset();
    }

    log.destroy();    
    return 0;
}