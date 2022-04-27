#include "boost/algorithm/string.hpp"
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "error_code.h"
#include "memory/xstr.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "database_host_server.h"

DatabaseHostServer::DatabaseHostServer(const XMQModeConf& conf)
    : XMQNode(), DatabaseNode(), modeconf{conf}, id{0}, 
    logid{std::string(DatabaseHostID).append("_log")}
{
    log.createNew("", false, 0);
}

DatabaseHostServer::~DatabaseHostServer()
{
    log.destroy();
}

int DatabaseHostServer::run(
    const DatabaseType& type/* = DatabaseType::DATABASE_TYPE_NONE*/, 
    const char* ip/* = nullptr*/, 
    const char* auth/* = nullptr*/, 
    const uint16_t port/* = 0*/)
{
    int ret{XMQNode::run()};

    if (Error_Code_Success == ret)
    {
        XStr xstr;
        DBModeConf conf;
        XMem().zero(&conf, sizeof(DBModeConf));
        conf.type = type;
        conf.id = ++id;
        conf.port = port;
		xstr.copy(ip, xstr.len(ip), conf.ip, 128);
		xstr.copy(auth, xstr.len(auth), conf.passwd, 64);
        ret = DatabaseNode::addConf(conf);

        if (Error_Code_Success == ret)
        {
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Run dvs host server name [ %s ], xmq_addr [ %s ], xmq_port [ %d ], type [ %d ].") 
                    % logid % modeconf.name % modeconf.ip % modeconf.port % static_cast<int>(modeconf.type)).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
    }
    
    return ret;
}

int DatabaseHostServer::stop()
{
    int ret{XMQNode::stop()};

    if (Error_Code_Success == ret)
    {
    }

    return ret;
}

void DatabaseHostServer::afterFetchOnlineStatusNotification(const bool online)
{
    const std::string log{
        (boost::format(
            "info://%s?command=add&severity=%d&log=Fetch message of online status [ %s ] notification.") 
            % logid % (online ? 0 : 1) % (online ? "online" : "offline")).str()};
    XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
}

void DatabaseHostServer::afterFetchServiceCapabilitiesNotification(const char* names/* = nullptr*/)
{
    const std::string log{
        (boost::format(
            "info://%s?command=add&severity=0&log=Fetch response message of service table [ %s ] notification.") 
            % logid % names).str()};
    XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
}

void DatabaseHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    Url requestUrl;
    int ret{requestUrl.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.proto().compare("config"))
        {
            config(from, requestUrl);
        }
        else
        {
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=1&log=Parsed request message with invalid url [ %s ] from [ %s ].") 
                    % logid % msg.c_str() % from).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
    }
    else
    {
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=2&log=Parse request message [ %s ] from [ %s ] failed, result [ %d ].") 
                % logid % msg.c_str() % from % ret).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }
}

void DatabaseHostServer::config(const std::string from, Url& url)
{
    std::string data;
    const std::vector<Parameter> params{url.parameters()};

    for (int i = 0; i != params.size(); ++i)
    {
        if (!params[i].key.compare("data"))
        {
            data = params[i].value;
        }
    }

    if (!data.empty())
    {
        auto o{boost::json::parse(data).as_object()};
        auto command{o.at("command").as_string()}, timestamp{o.at("timestamp").as_string()};
        std::string out;

        if (!command.compare("mec.db.dvs.query"))
        {
            o["error"] = "0";
            char* buf{
                DatabaseNode::read(id, "hgetall mec.dvs.config")};

            if (buf)
            {
                boost::json::array arr;
                std::vector<std::string> items;
                boost::split(items, std::string(buf), boost::is_any_of("_"));

                for (int i = 0; i != items.size(); ++i)
                {
                    arr.emplace_back(items[i]);
                }
                o["devices"] = arr;
                boost::checked_array_delete(buf);
            }
        }
        else if(!command.compare("mec.db.rsu.query"))
        {
            o["error"] = "0";
            char* buf{
                DatabaseNode::read(id, "hgetall mec.rsu.config")};

            if (buf)
            {
                boost::json::array arr;
                std::vector<std::string> items;
                boost::split(items, std::string(buf), boost::is_any_of("_"));

                for (int i = 0; i != items.size(); ++i)
                {
                    arr.emplace_back(items[i]);
                }
                o["devices"] = arr;
                boost::checked_array_delete(buf);
            }
        }
        else if (!command.compare("mec.db.dvs.add"))
        {
            const std::string did{o.at("id").as_string().c_str()};
            o.erase("command");
            o.erase("timestamp");
            out = boost::json::serialize(o);
            int ret{
                DatabaseNode::write(
                    id, 
                    (boost::format("hset mec.dvs.config %s %s") % did % out).str().c_str())};

            o.clear();
            o["command"] = command;
            o["error"] = (boost::format("%d") % ret).str();
            o["timestamp"] = timestamp;
        }
        else if (!command.compare("mec.db.rsu.add"))
        {
            const std::string did{o.at("id").as_string().c_str()};
            o.erase("command");
            o.erase("timestamp");
            out = boost::json::serialize(o);
            int ret{
                DatabaseNode::write(
                    id, 
                    (boost::format("hset mec.rsu.config %s %s") % did % out).str().c_str())};

            o.clear();
            o["command"] = command;
            o["error"] = (boost::format("%d") % ret).str();
            o["timestamp"] = timestamp;
        }
        else if (!command.compare("mec.db.dvs.remove"))
        {
            const std::string did{o.at("id").as_string().c_str()};
            o.erase("id");
            out = boost::json::serialize(o);
            int ret{
                DatabaseNode::write(
                    id, 
                    (boost::format("hdel mec.dvs.config %s") % did).str().c_str())};
            o["error"] = (boost::format("%d") % ret).str();
        }
        else if (!command.compare("mec.db.rsu.remove"))
        {
            const std::string did{o.at("id").as_string().c_str()};
            o.erase("id");
            out = boost::json::serialize(o);
            int ret{
                DatabaseNode::write(
                    id, 
                    (boost::format("hdel mec.rsu.config %s") % did).str().c_str())};
            o["error"] = (boost::format("%d") % ret).str();
        }
        else if(!command.compare("mec.db.position.query"))
        {
            char* buf{
                DatabaseNode::read(id, "get mec.position.config")};

            if (buf)
            {
                //longitude;latitude;elevation
                std::vector<std::string> items;
                boost::split(items, std::string(buf), boost::is_any_of(";"));
                if (3 == items.size())
                {
                    double d_longitude{ std::strtod(items[0].c_str(), NULL) };
                    double d_latitude{ std::strtod(items[1].c_str(), NULL) };
                    int d_elevation{ atoi(items[2].c_str()) };

                    o["longitude"] = d_longitude;
                    o["latitude"] = d_latitude;
                    o["elevation"] = d_elevation;
                }
                boost::checked_array_delete(buf);
            }

            o["error"] = (buf ? "0" : "-9");
        }
        else if(!command.compare("mec.db.position.set"))
        {
            const double longitude{o["longitude"].as_double()}, latitude{o["latitude"].as_double()};
            const int64_t elevation{o["elevation"].as_int64()};
            //longitude;latitude;elevation
            const std::string pos{
                (boost::format("%.7f;%.7f;%d") % longitude % latitude % elevation).str()};
            int ret{
                DatabaseNode::write(
                    id, 
                    (boost::format("set mec.position.config %s") % pos).str().c_str())};
            o.erase("longitude");
            o.erase("latitude");
            o.erase("elevation");
            o["error"] = (boost::format("%d") % ret).str();
        }

        out = boost::json::serialize(o);
        std::string url{
            (boost::format("config://%s?data=%s") % from % out).str()};
        XMQNode::send(modeconf.id, url.c_str(), url.length());
    }
}
