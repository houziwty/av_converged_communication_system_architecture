#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "boost/make_shared.hpp"
#include "libavpkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), Libasio(), Libdvs(), Libavparser(), log{flog}, did{0}, sid{0}, 
    xid{0}, logid{std::string(DVSHostID).append("_log")}
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

        //Open stream media server.
        ASIOModeConf ioconf;
        ioconf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        ioconf.port = 60820;
        ioconf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
        ret = Libasio::addConf(ioconf);

        if (Error_Code_Success == ret)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run dvs streaming server [ %u ] successfully.", ioconf.port);
        }
        else
        {
             log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Run dvs streaming server [ %u ] failed, reuslt = [ %d ].", ioconf.port, ret);
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
            "info://%s?data={\"command\":\"add\",\"severity\":0,\"text\":\"Run dvs host server name [ %s ], xmq_addr [ %s ], xmq_port [ %d ], type [ %d ].\"}") 
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

int Server::sendframe(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    return (0 < id && data && 0 < bytes ? Libasio::send(id, data, bytes) : Error_Code_Invalid_Param);
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
            "Fetch dvs host server online status [ %s ].", online ? "online" : "offline");
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch dvs host server online status [ %s ].", online ? "online" : "offline");
    }

    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":0,\"text\":\"DVS host server online status [ %s ].\"}")
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
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/,  
    const int32_t e/* = 0*/)
{
    const uint32_t sessionid{++sid};

    if (!e)
    {
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
    return 0;
}

void Server::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        //解析视频播放的URL
        //用完就可以释放
        //用IO的ID来标识连接
        AVParserModeConf conf{id, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER};
        Libavparser::addConf(conf);
        Libavpkt avpkt;
        avpkt.input(data, bytes);
        Libavparser::input(id, &avpkt);
        Libavparser::removeConf(id);
    }
    else
    {
        int ret{Libasio::removeConf(id)};
        std::vector<SessionPtr> sps{sss.values()};
        for (int i = 0; i != sps.size(); i++)
        {
            if (Error_Code_Success == sps[i]->removeTarget(id))
            {
                break;
            }
        }

        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Remove stream session [ %d ] while catch error code [ %d ].", id, e);
        const std::string text{
            (boost::format(
                "info://%s?data={\"command\":\"add\",\"severity\":1,\"text\":\"Remove stream session [ %d ] while catch error code [ %d ]\".")
                % logid % id % e).str() };
        Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
    }
}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{}

void Server::processConfigRequest(
    const char* name/* = nullptr*/, 
    const char* req/* = nullptr*/)
{
    if (name && req)
    {
        auto o{boost::json::parse(req).as_object()};
        auto command{o.at("command").as_string()}, timestamp{o.at("timestamp").as_string()};
        std::string out, from{name};

        if (!command.compare("mec.dvs.add"))
        {
            std::string id;
            auto factory{ o.at("factory").as_string() },
                name{ o.at("name").as_string() },
                ip{ o.at("ip").as_string() },
                port{ o.at("port").as_string() },
                user{ o.at("user").as_string() },
                passwd{ o.at("passwd").as_string() },
                timestamp{ o.at("timestamp").as_string() };
            if (o.contains("id"))
            {
                id = o.at("id").as_string().c_str();
            }

            addDVS(
                factory.c_str(), id, ip.c_str(), port.c_str(), user.c_str(), passwd.c_str(), 
                from.c_str(), name.c_str(), timestamp.c_str());
        }
        else if (!command.compare("mec.dvs.remove"))
        {
            auto did{o.at("id").as_string()};
            removeDVS(from, did.c_str(), timestamp.c_str());
        }
    }
}

int Server::addDVS(
    const std::string& factory, 
    const std::string& id, 
    const std::string& ip, 
    const std::string& port, 
    const std::string& user, 
    const std::string& passwd, 
    const std::string& from, 
    const std::string& name, 
    const std::string& timestamp)
{
    int ret{
        !factory.empty() && !ip.empty() && !port.empty() && !user.empty() && 
        !passwd.empty() && !from.empty() && !name.empty() && !timestamp.empty() ? 
        Error_Code_Success : 
        Error_Code_Invalid_Param};
    uint32_t channels{ 0 };
    DVSModeConf conf;
    memset(&conf, 0, sizeof(DVSModeConf));

    if (Error_Code_Success == ret)
    {
        const uint32_t temp_did{ (uint32_t)atoi(id.c_str()) };
        conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_NONE;
        if (!factory.compare("0"))
        {
            conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_HK;
        }
        else if (!factory.compare("1"))
        {
            conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_DH;
        }
        conf.module = DVSModuleType::DVS_MODULE_TYPE_NONE;
        //设备ID从1开始原子计数
        conf.id = (id.empty() ? ++did : temp_did);
        XMem().copy(user.c_str(), user.length(), conf.user, 128);
        XMem().copy(passwd.c_str(), passwd.length(), conf.passwd, 64);
        XMem().copy(ip.c_str(), ip.length(), conf.ip, 128);
        conf.port = atoi(port.c_str());
        int ret{Libdvs::addConf(conf)};

        //Reply
        boost::json::object o;
        o["command"] = "mec.dvs.add";
        o["error"] = (boost::format("%d") % ret).str();
        o["id"] = (boost::format("%d") % conf.id).str();
        if (Error_Code_Success == Libdvs::queryConf(conf.id, DVSConfParamType::DVS_CONF_PARAM_TYPE_CHANNEL_NUMBER, &channels))
        {
            o["channels"] = (boost::format("%d") % channels).str();
        }
        o["timestamp"] = (boost::format("%llu") % timestamp).str();
        if (!id.empty())
        {
            o["factory"] = factory;
            o["ip"] = ip;
            o["port"] = port;
            o["user"] = user;
            o["passwd"] = passwd;
            o["name"] = name;

            if (did <= temp_did)
            {
                did = temp_did;
            }
        }
        const std::string out{boost::json::serialize(o)};
        const std::string rep{
            (boost::format("config://%s?data=%s") % from % out).str()};
        Libxmq::send(xid, rep.c_str(), rep.length(), from.c_str());
    }

    //Terminal
    if(Error_Code_Success == ret)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Add new device [ %d_%s_%s_%u_%s_%s_%d ] successfully.", 
            conf.id, name.c_str(), conf.ip, conf.port, conf.user, conf.passwd, channels);
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Add new device [ %d_%s_%s_%u_%s_%s_%d ] failed, result = [ %d ].", 
            conf.id, name.c_str(), conf.ip, conf.port, conf.user, conf.passwd, channels, ret);
    }

    //Log
    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":%d,\"text\":\"Add new device [ %d_%s_%s_%u_%s_%s_%d ] result [ %d ].\"}")
            % logid % (ret ? 1 : 0) % conf.id % name.c_str() % conf.ip % conf.port % conf.user % conf.passwd % channels % ret).str() };
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
    
    return ret;
}

int Server::removeDVS(
    const std::string& from, 
    const std::string& id, 
    const std::string& timestamp)
{
    int ret{!from.empty() && !id.empty() && !timestamp.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = Libdvs::removeConf(atoi(id.c_str()));
        
        boost::json::object o;
        o["command"] = "mec.dvs.remove";
        o["error"] = (boost::format("%d") % ret).str();
        o["timestamp"] = timestamp;
        const std::string out{boost::json::serialize(o)};
        const std::string rep{
                (boost::format("config://%s?data=%s") % from % out).str()};
        Libxmq::send(xid, rep.c_str(), rep.length(), from.c_str());
    }

    //Terminal
    if(Error_Code_Success == ret)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Remove device [ %s ] successfully.", id.c_str());
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Remove device [ %s ] failed, result = [ %d ].", id.c_str(), ret);
    }

    //Log
    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":%d,\"text\":\"Remove device [ %s ] result [ %d ].\"}")
            % logid % (ret ? 1 : 0) % id.c_str() % ret).str() };
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());

    return ret;
}

void Server::afterPolledDVSDataNotification(
    const uint32_t id/* = 0*/, 
    const int32_t channel/* = 0*/, 
    const uint32_t type/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint32_t bytes/* = 0*/)
{
    const std::string sid{ (boost::format("%d_%d") % id % channel).str() };
    SessionPtr sp{ sss.at(sid) };
    if (!sp)
    {
        sp = boost::make_shared<Session>(*this);
        if (sp)
        {
            sss.add(sid, sp);
        }
    }

    if (sp)
    {
        sp->input(data, bytes);
    }
}

void Server::afterPolledDVSExceptionNotification(
    const uint32_t id/* = 0*/, 
    const int32_t error/* = 0*/)
{
    //Terminal
    log.write(
        SeverityLevel::SEVERITY_LEVEL_WARNING,
        "Fetch exception code [ %d ] from device [ %d ].", error, id);

    //Log
    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":1,\"text\":\"Fetch exception code [ %d ] from device [ %d ].\"}")
            % logid % error % id).str() };
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
}

void Server::afterParsedDataNotification(
    const uint32_t id/* = 0*/, 
    const void* avpkt/* = nullptr*/)
{
    Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
    Url url;
    int ret{url.parse(pkt->data(), pkt->bytes())};
    int command{-1}, did{-1}, cid{-1};

    if(Error_Code_Success == ret)
    {
        //realplay://1?command=1&channel=1&stream=0
        did = atoi(url.host().c_str());
        const std::vector<Parameter> params{url.parameters()};

        for(int i  = 0; i != params.size(); ++i)
        {
            if(!params[i].key.compare("data"))
            {
                auto o{ boost::json::parse(params[i].value).as_object() };
                auto channel{ o.at("channel").as_string() };
                cid = atoi(channel.c_str());

                const std::string sid{ (boost::format("%d_%d") % did % cid).str() };
                SessionPtr sp{ sss.at(sid) };
                if (sp)
                {
                    ret = sp->addTarget(id);
                }

                break;
            }
        }
    }

     //Terminal
    if(Error_Code_Success == ret)
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Play live stream [ %d_%d ] from session [ %d ] successfully.", did, cid, id);
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Play live stream [ %d_%d ] from session [ %d ] failed, result = [ %d ].", did, cid, id);
    }

    //const std::string text{
    //    (boost::format(
    //        "info://%s?data={\"command\":\"add\",\"severity\":%d,\"text\":\"Play live stream [ %d_%d ] from session [ %d ], result [ %d ].\"}")
    //        % logid % (ret ? 1 : 0) % did % cid % ret).str() };
    //Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
}
