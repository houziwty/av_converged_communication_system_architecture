#include "boost/format.hpp"
#include "boost/json.hpp"
#include <drogon/drogon.h>
using namespace drogon; 
#include "error_code.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url.h"
using namespace framework::utils::data;
#include "controller/device/device.h"
#include "controller/capabilities/capabilities.h"
using namespace api::v1;
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), log{flog}, xid{0}, 
    sessionIDNumber{0}, logid{std::string(HttpHostID).append("_log")}
{}

Server::~Server()
{}

int Server::run(const XMQNodeConf& conf)
{
    int ret{Libxmq::addNode(conf)};

    if (Error_Code_Success == ret)
    {
        bool httpValid{false};
        xid = conf.id;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Connect to xmq server [ %s : %u ] successfully.", conf.ip, conf.port);

        ThreadPool().get_mutable_instance().createNew(
            boost::bind(&Server::httpWorkerThread, this));
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
            
            app().quit();
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
            "Fetch http host server online status [ %s ].", online ? "online" : "offline");
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch http host server online status [ %s ].", online ? "online" : "offline");
    }

    const std::string text{
        (boost::format(
            "info://%s?data={\"command\":\"add\",\"severity\":0,\"text\":\"HTTP host server online status [ %s ].\"}")
            % logid % (online ? "online" : "offline")).str()};
    Libxmq::send(xid, text.c_str(), text.length(), logid.c_str());
}

void Server::afterFetchServiceCapabilitiesNotification(const char* names/* = nullptr*/)
{
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
        "Fetch system online names of servers [ %s ].", names);
}

void Server::httpWorkerThread()
{
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, "Start http service.");
    
    loadHttpController();
    //设置应答消息server字段
    app().setServerHeaderField("clt/0.0.1").setUploadPath("./uploads").addListener("127.0.0.1", 18080).run();
}

void Server::loadHttpController()
{
    app().registerController(std::make_shared<api::v1::Device>(*this));
    app().registerController(std::make_shared<api::v1::Capabilities>());
}

void Server::getDvslist(std::string& rep)
{
    XTime tm;
    const uint64_t now{tm.tickcount()};
    const std::string msg{
        (boost::format("config://external_deamon_host_server?data={\"command\":\"mec.dvs.query\",\"timestamp\":\"%llu\"}") % now).str()};
    syncXMQMessageProcess(now, msg, rep);
}

void Server::addDvs(const std::string& req, std::string& rep)
{
    XTime tm;
    const uint64_t now{tm.tickcount()};
    auto o{boost::json::parse(req).as_object()};
    o["command"] = "mec.dvs.add";
    o["timestamp"] = std::to_string(now);
    const std::string msg{
        (boost::format("config://external_deamon_host_server?data=%s") % boost::json::serialize(o)).str()};
    syncXMQMessageProcess(now, msg, rep);
}

void Server::removeDvs(const std::string& req, std::string& rep)
{
    XTime tm;
    const uint64_t now{tm.tickcount()};
    auto o{boost::json::parse(req).as_object()};
    o["command"] = "mec.dvs.remove";
    o["timestamp"] = std::to_string(now);
    const std::string msg{
        (boost::format("config://external_deamon_host_server?data=%s") % boost::json::serialize(o)).str()};
    syncXMQMessageProcess(now, msg, rep);
}

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

        const uint64_t last{atoll(timestamp.c_str())};
        const uint64_t diff{XTime().tickcount() - last};
        if (8000 > diff)
        {
            asyncMessages.add(last, req);
        }
    }
}

int Server::syncXMQMessageProcess(
    const uint64_t id, 
    const std::string& req, 
    std::string& rep)
{
    int ret{Error_Code_Success};
    XTime tm;
    const uint64_t now{tm.tickcount()};
    rep.clear();

    if (Error_Code_Success == Libxmq::send(xid, req.c_str(), req.length(), "external_deamon_host_server"))
    {
        while(10000 > tm.tickcount() - now)
        {
            rep = asyncMessages.at(id);
            if (!rep.empty())
            {
                break;
            }
            
            tm.sleep(1000);
        }

        ret = Error_Code_Operate_Failure;
    }

    return ret;
}
