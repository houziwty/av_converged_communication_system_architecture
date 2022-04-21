#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), Libfdfs(), Libasio(), log{flog}, sid{0}, 
    xid{0}, logid{std::string(StorageHostID).append("_log")}
{}

Server::~Server()
{
    Libfdfs::destroy();
}

int Server::run(
    const XMQNodeConf& conf, 
    const char* fdfsconf/* = nullptr*/)
{
    int ret{Libxmq::addNode(conf)};

    if(Error_Code_Success == ret)
    {
        xid = conf.id;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Connect to xmq server [ %s : %u ] successfully.", conf.ip, conf.port);

        //Init FastDFS connection.
        ret = Libfdfs::init(fdfsconf);
        if (Error_Code_Success == ret)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run fast dfs server successfully.");

            //Open stream download server.
            ASIOModeConf ioconf;
            ioconf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            ioconf.port = 60927;
            ioconf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            ret = Libasio::addConf(ioconf);

            if (Error_Code_Success == ret)
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "Run storage streaming server [ %u ] successfully.", ioconf.port);
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_ERROR, 
                    "Run storage streaming server [ %u ] failed, reuslt = [ %d ].", ioconf.port, ret);
            }
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Run fast dfs server failed, result = [ %d ].", ret);
        }
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR,
            "Connect to xmq server [ %s : %u ] failed, result = [ %d ].", conf.ip, conf.port, ret);
    }

    const std::string text{
        (boost::format(
            "info://%s?command=add&severity=0&log=Run storage host server name [ %s ], xmq_addr [ %s ], xmq_port [ %d ], type [ %d ].") 
            % logid % conf.name % conf.ip % conf.port % static_cast<int>(conf.type)).str()};
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
    
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

            ret = Libfdfs::destroy();
            if (Error_Code_Success == ret)
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "Stop fast dfs server successfully.");
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_ERROR, 
                    "Stop fast dfs server failed, result = [ %d ].", ret);
            }
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

int Server::send(
    const uint32_t sid/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{0 < sid && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        while(Error_Code_Success == Libasio::send(sid, data, bytes))
        {
            XTime().sleep(10);
        }

        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Send request of realplay stream [ %d ] with [ %s ].",
            sid, (const char*)data);
    }

    return ret;
}

void Server::afterFetchOnlineStatusNotification(const bool online)
{
	log.write(
		online ? SeverityLevel::SEVERITY_LEVEL_INFO : SeverityLevel::SEVERITY_LEVEL_WARNING,
		"Fetch message of online status [ %s ] notification.",
		online ? "online" : "offline");
}

void Server::afterFetchServiceCapabilitiesNotification(
    const char** names/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    std::string text;

    for (int i = 0; i != number; ++i)
    {
        text += ("[ " + std::string(names[i]) + " ]");
    }

	log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
		"Fetch response message of service table [ %s ] notification.",
        text.c_str());
}

void Server::afterPolledXMQDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/)
{
    Url url;
    int ret{url.parse((const char*)data)};

    //Only configure message work.
    if(Error_Code_Success == ret)
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
        const std::string text{(const char*)data};
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Parse request message [ %s ] from [ %s ] failed, result [ %d ].",
            text.c_str(), name, ret);
    }
}

uint32_t Server::afterFetchIOAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const int32_t e/* = 0*/)
{
    //创建存储服务客户端连接会话
    //该会话处理存储数据发送
    uint32_t sessionid{0};

    if (!e)
    {
        sessionid = ++sid;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Fetch connection [ %d ] from remote [ %s : %u ] successfully.", sessionid, ip, port);
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch connection [ %d ] from remote [ %s : %u ] failed, result = [ %d ].", sessionid, ip, port, e);
    }
    
    return sessionid;
}

uint32_t Server::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    //创建实时视频数据读取会话
    //与DVS设备接入主机连接
    uint32_t sessionid{0};

    if (!e && user)
    {
        sessionid = ++sid;
        const std::string id{(const char*)user};
        const std::size_t pos{id.find_first_of('_')};
        const std::string did{id.substr(0, pos)};
        const std::string cid{id.substr(pos + 1, id.length())};
        UploadSessionPtr sess{
            boost::make_shared<UploadSession>(*this, sessionid)};

        if (sess && Error_Code_Success == sess->run((uint32_t)atoi(did.c_str()), (uint32_t)atoi(cid.c_str())))
        {
            uploadSessions.add(sessionid, ptr);
        }

        boost::checked_array_delete(user);
    }

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
        if (0 < id && data && 0 < bytes)
        {
            UploadSessionPtr ptr{uploadSessions.at(id)};

            if (ptr)
            {
                ptr->input(data, bytes);
            }
        }
    }
    else
    {
        uploadSessions.remove(id);
    }
}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

void Server::processConfigRequest(
    const char* name/* = nullptr*/, 
    const char* req/* = nullptr*/)
{
    if (name && req)
    {
        auto o{boost::json::parse(req).as_object()};
        auto command{o.at("command").as_string()}, timestamp{o.at("timestamp").as_string()};
        std::string out;

        if (!command.compare("mec.realplay.add"))
        {
            auto did{o.at("id").as_string()}, 
                channel{o.at("channel").as_string()};
            const std::string id{
                (boost::format("%s_%s") % did.c_str() % channel.c_str()).str()};

            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = 60820;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
            conf.tcp.ip = "127.0.0.1";
            conf.tcp.user = XStr().alloc(id.c_str(), id.length());
            int ret{ ASIONode::addConf(conf) };

            //Reply
            boost::json::object o;
            o["command"] = "mec.realplay.add";
            o["error"] = (boost::format("%d") % ret).str();
            o["timestamp"] = timestamp;
            const std::string out{boost::json::serialize(o)};
            const std::string rep{
                (boost::format("config://%s?data=%s") % from % out).str()};
            Libxmq::send(xid, rep.c_str(), rep.length(), name);

            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Fetch request for creating realplay stream [ %d_%d ].", did.c_str(), channel.c_str());
        }
        else if (!command.compare("mec.realplay.remove"))
        {
            auto did{o.at("id").as_string()}, 
                channel{o.at("channel").as_string()};
        }
    }
}
