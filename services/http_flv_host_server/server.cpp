#include "boost/format.hpp"
#include "boost/json.hpp"
#include "error_code.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"

static int handler(void* param, int type, const void* data, size_t bytes, uint32_t timestamp)
{
    return 0;
}

Server::Server(FileLog& flog)
    : Libxmq(), log{flog}, xid{0}, 
    sessionIDNumber{0}, logid{std::string(HttpFlvHostID).append("_log")}, connected{false}, flv{nullptr}
{}

Server::~Server()
{}

int Server::run(const XMQNodeConf& conf)
{
    int ret{Libxmq::addNode(conf)};

    if (Error_Code_Success == ret)
    {
        xid = conf.id;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Connect to xmq server [ %s : %u ] successfully.", conf.ip, conf.port);

        AVParserModeConf parser;
        parser.type = AVParserType::AV_PARSER_TYPE_BUFFER_PARSER;
        parser.id = 1000;
        parser.cache = 3 * 1024 * 1024;
        Libavparser::addConf(parser);

        ASIOModeConf conf;
        conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        conf.port = 18081;
        conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
        Libasio::addConf(conf);

        flv = flv_muxer_create(&handler, this);
    }
    
    return ret;
}

int Server::stop()
{
    int ret{0 < xid ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        ret = Libxmq::removeNode(xid);

        if (Error_Code_Success == ret)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Disconnect xmq connection [ %d ] successfully.", xid);
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Disconnect xmq connection [ %d ] failed, result = [ %d ].", xid, ret);
        }
    }

    return ret;
}

void Server::afterPolledXMQDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/)
{
    Url url;
    int ret{url.parse(data, bytes)};

    //Only configure message work.
    if(Error_Code_Success == ret && !url.proto().compare("config"))
    {
        const std::vector<Parameter> params{url.parameters()};

        for (int i = 0; i != params.size(); ++i)
        {
            if (!params[i].key.compare("data"))
            {
                processConfigRequest(name, params[i].value.c_str());
                break;
            }
        }
    }
    else
    {
        const std::string text{ (const char*)data, bytes };
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Parse request message [ %s ] from [ %s ] failed, result [ %d ].",
            text.c_str(), name, ret);
    }
}

void Server::afterFetchOnlineStatusNotification(const bool online)
{
    if (online)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Fetch http-flv host server online status [ %s ].", online ? "online" : "offline");
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch http-flv host server online status [ %s ].", online ? "online" : "offline");
    }

    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":0,\"text\":\"HTTP-Flv host server online status [ %s ].\"}")
            % logid % (online ? "online" : "offline")).str()};
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
}

void Server::afterFetchServiceCapabilitiesNotification(const char* names/* = nullptr*/)
{
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
        "Fetch system online names of servers [ %s ].", names);
}

uint32_t Server::afterFetchIOAcceptedEventNotification(
    const char* remoteIP/* = nullptr*/, 
    const uint16_t remotePort/* = 0*/, 
    const uint16_t localPort/* = 0*/, 
    const int32_t e/* = 0*/)
{
    Libhttp::addSession(10);
    return 10;
}

uint32_t Server::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    //创建实时视频数据读取会话
    //与DVS设备接入主机连接
    uint32_t sessionid{1};

    if (!e && user)
    {
        // sessionid = ++sid;
        // const std::string ids{(const char*)user};
        // std::vector<std::string> items;
        // boost::split(items, ids, boost::is_any_of("_"));
        // SessionPtr ptr{
        //     boost::make_shared<UploadSession>(
        //         *this, sessionid, (uint32_t)atoi(items[1].c_str()), (uint32_t)atoi(items[2].c_str()))};

        // if (ptr && Error_Code_Success == ptr->run())
        // {
        //     sessions.add(sessionid, ptr);
        //     tasks.add((uint32_t)atoi(items[0].c_str()), ptr);
        //     log.write(
        //         SeverityLevel::SEVERITY_LEVEL_INFO,
        //         "Add new upload session with ids [ %s ] successfully.", ids.c_str());
        // }
        // else
        // {
        //     log.write(
        //         SeverityLevel::SEVERITY_LEVEL_WARNING,
        //         "Add new upload session with ids [ %s ] failed.", ids.c_str());
        // }

        // boost::checked_array_delete((char*)user);
    }

    connected = true;
    return sessionid;
}

void Server::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        if (10 == id && data && 0 < bytes)
        {
            Libhttp::input(10, data, bytes);
        }
        else if (1 == id && data && 0 < bytes)
        {
            Libavpkt avpkt;
            avpkt.input(data, bytes);
            Libavparser::input(1000, &avpkt);
        }
    }
    else
    {
        Libhttp::removeSession(id);
    }
}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

void Server::afterParsedDataNotification(
    const uint32_t id/* = 0*/, 
    const void* avpkt/* = nullptr*/)
{
    if (0 < id && avpkt)
    {
        Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
        flv_muxer_avc(flv, pkt->data(), pkt->bytes(), 40, 40);

        // static FILE* fd{nullptr};
        // if (!fd)
        // {
        //     fd = fopen("/mnt/d/test.264", "wb+");
        // }
        // fwrite(pkt->data(), pkt->bytes(), 1, fd);
    }
}

void Server::afterFetchHttpResponseNotification(
    const uint32_t id/* = 0*/, 
    const char* response/* = nullptr*/, 
    const bool close/* = false*/)
{

}

void Server::afterFetchHttpRequestNotification(
    const uint32_t id/* = 0*/, 
    const char* url/* = nullptr*/, 
    const char* contentType/* = nullptr*/, 
    const char* content/* = nullptr*/)
{
    if (0 < id && url && contentType && content)
    {
        ASIOModeConf conf;
        conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        conf.port = 60820;
        conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
        conf.tcp.ip = "192.168.2.83";
        conf.tcp.user = nullptr;

        int ret{Libasio::addConf(conf)};
        const std::string uri{"realplay://1?data={\"channel\":\"1\",\"command\":\"1\",\"stream\":\"0\"}"};
        const uint64_t bytes{ 32 + uri.length() };
        char* data{ new char[bytes] };
        *((uint32_t*)data) = 0xFF050301;
        *((uint32_t*)(data + 4)) = 0;
        *((uint32_t*)(data + 8)) = 0;
        *((uint32_t*)(data + 12)) = uri.length();
        *((uint32_t*)(data + 16)) = 0;
        *((uint64_t*)(data + 24)) = 0;
        memcpy(data + 32, uri.c_str(), uri.length());

        while (1)
        {
            if (!connected)
            {
                XTime().sleep(100);
                continue;
            }

            if(!Libasio::send(1, data, bytes));
            {
                break;
            }
        }
    }
}

// void Server::loadHttpController()
// {
//     app().registerHandler(
//         "/stream/live/flv/{1}-{2}.flv",
//         [&](const HttpRequestPtr &req,
//            std::function<void(const HttpResponsePtr &)> &&callback) {
//             auto resp = HttpResponse::newHttpResponse();
//             resp->setContentTypeString("video/x-flv");
//             resp->setCloseConnection(false);
//             callback(resp);

//             const std::string path{req->getPath()};
//             const std::size_t blackSplashPos{path.find_last_of('/')}, 
//                 midLinePos{path.find_first_of('-', blackSplashPos)}, 
//                 dotSplashPos{path.find_first_of('.', midLinePos)};
                



//             while(1)
//             {
//                 const std::size_t num{avpkts.size()};
//                 if (0 < num)
//                 {
//                     Libavpkt* pkt{avpkts.front()};
//                     auto resp = HttpResponse::newHttpResponse();
//                     resp->setContentTypeString("video/x-flv");
//                     resp->setCloseConnection(false);
//                     resp->setBody(std::string((char*)pkt->data(), pkt->bytes()));
//                     callback(resp);

//                     mtx.lock();
//                     avpkts.erase(avpkts.begin());
//                     mtx.unlock();
//                 }
//             }
//         },
//         {Get});
// }

void Server::processConfigRequest(
    const char* name/* = nullptr*/, 
    const char* req/* = nullptr*/)
{
    if (name && req)
    {
        auto o{boost::json::parse(req).as_object()};
        auto command{o.at("command").as_string()}, timestamp{o.at("timestamp").as_string()};
        std::string out, from{name};

        // if (!command.compare("mec.dvs.add"))
        // {
        //     std::string id;
        //     auto factory{ o.at("factory").as_string() },
        //         name{ o.at("name").as_string() },
        //         ip{ o.at("ip").as_string() },
        //         port{ o.at("port").as_string() },
        //         user{ o.at("user").as_string() },
        //         passwd{ o.at("passwd").as_string() },
        //         timestamp{ o.at("timestamp").as_string() },
        //         sn{o.at("sn").as_string()};
        //     if (o.contains("id"))
        //     {
        //         id = o.at("id").as_string().c_str();
        //     }
        // }
        // else if (!command.compare("mec.dvs.remove"))
        // {
        //     const std::string e{o.at("error").as_string().c_str()};
        //     const std::string timestamp{o.at("timestamp").as_string().c_str()};
        // }
        // else if (!command.compare("mec.dvs.query"))
        // {
        //     const std::string e{o.at("error").as_string().c_str()};
        //     const std::string timestamp{o.at("timestamp").as_string().c_str()};
        //     o.erase("command");
        //     o.erase("error");
        //     o.erase("timestamp");
        //     out = boost::json::serialize(o);
        // }
        // else
        // {
        //     return;
        // }
    }
}
