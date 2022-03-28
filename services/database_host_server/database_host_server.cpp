#include "boost/checked_delete.hpp"
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
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

void DatabaseHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    std::string text;

    for (int i = 0; i != number; ++i)
    {
        text += ("[ " + std::string(infos[i].name) + " ]");
    }

    const std::string log{
        (boost::format(
            "info://%s?command=add&severity=0&log=Fetch response message of service table [ %s ] notification.") 
            % logid % text.c_str()).str()};
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
            processDatabaseRequest(from, requestUrl);
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

void DatabaseHostServer::processDatabaseRequest(const std::string from, Url& url)
{
    std::string command, name, data;
    const std::vector<Parameter> params{url.parameters()};

    for (int i = 0; i != params.size(); ++i)
    {
        if (!params[i].key.compare("command"))
        {
            command = params[i].value;
        }
        else if (!params[i].key.compare("name"))
        {
            name = params[i].value;
        }
        else if (!params[i].key.compare("data"))
        {
            data = params[i].value;
        }
    }

    if (!command.compare("query"))
    {
    }
    else if (!command.compare("add"))
    {
        DatabaseNode::write(id, name.c_str(), data.c_str());
    }
}
