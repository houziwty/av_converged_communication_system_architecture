#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "av_pkt.h"
using namespace module::av::stream;
#include "json/json.h"
using namespace framework::utils::data;
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "url/url.h"
using namespace framework::utils::data;
#include "dvs_host_server.h"

DvsHostServer::DvsHostServer(const XMQModeConf& conf)
    : XMQNode(), ASIONode(), DVSNode(), 
    modeconf{conf}, deviceNumber{0}, streamNumber{0}, 
    logid{std::string(DVSHostID).append("_log")}
{}

DvsHostServer::~DvsHostServer()
{}

int DvsHostServer::run()
{
    int ret{XMQNode::run()};

    if (Error_Code_Success == ret)
    {
        ret = ASIONode::run();

        if (Error_Code_Success == ret)
        {
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Run dvs host server name [ %s ], xmq_addr [ %s ], xmq_port [ %d ], type [ %d ].") 
                    % logid % modeconf.name % modeconf.ip % modeconf.port % static_cast<int>(modeconf.type)).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());

            //Open stream media server
            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = 60820;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            ASIONode::addConf(conf);
        }
    }
    
    return ret;
}

int DvsHostServer::stop()
{
    int ret{XMQNode::stop()};

    if (Error_Code_Success == ret)
    {
        ret = ASIONode::stop();
    }

    return ret;
}

void DvsHostServer::afterFetchOnlineStatusNotification(const bool online)
{
    const std::string log{
        (boost::format(
            "info://%s?command=add&severity=%d&log=Fetch message of online status [ %s ] notification.") 
            % logid % (online ? 0 : 1) % (online ? "online" : "offline")).str()};
    XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
}

void DvsHostServer::afterFetchServiceCapabilitiesNotification(
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

void DvsHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    Url url;
    int ret{url.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!url.proto().compare("config"))
        {
            config(from, url);
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

uint32_t DvsHostServer::afterFetchAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/,  
    const int32_t e/* = 0*/)
{
    if (1)
    {
        WriteLock wl{mtx};
        ++streamNumber;
    }

    if (!e && 0 < streamNumber)
    {
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=0&log=Connected stream session [ %u ] from [ %s : %u ] successfully.") 
                % logid % streamNumber % ip % port).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }
    else
    {
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=1&log=Connected stream session [ %u ] from [ %s : %u ] failed, result [ %d ].") 
                % logid % streamNumber % ip % port % e).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }

    return streamNumber;
}

uint32_t DvsHostServer::afterFetchConnectedEventNotification(const int32_t e/* = 0*/)
{
    return 0;
}

void DvsHostServer::afterPolledReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        DVSStreamSessionPtr sess{sessions.at(id)};
        if (!sess)
        {
            sess = boost::make_shared<DvsStreamSession>(*this, modeconf, id);
            if (sess)
            {
                sessions.add(id, sess);
            }
        }

        if (sess)
        {
            sess->recv((const void*)data, bytes);
        }
    }
    else
    {
        //会话异常
        ASIONode::removeConf(id);
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=1&log=Remove stream session [ %u ] while throw exception code [ %d ].") 
                % logid % id % e).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }
}

void DvsHostServer::afterPolledSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{}

void DvsHostServer::config(const std::string from, Url& url)
{
    const std::vector<Parameter> parameters{url.parameters()};
    const std::string host{url.host()};
    std::string command, data;

    for(int i = 0; i != parameters.size(); ++i)
    {
        if (!parameters[i].key.compare("command"))
        {
            command = parameters[i].value;
        }
		else if (!parameters[i].key.compare("data"))
		{
            data = parameters[i].value;
		}
    }

    if (!command.compare("query"))
    {
        query(from, data);
    }
    else if (!command.compare("add"))
    {
        add(from, data);
    }
    else if (!command.compare("remove"))
    {
        remove(from, data);
    }
}

int DvsHostServer::add(const std::string& from, const std::string& json)
{
    int ret{from.empty() || json.empty() ? Error_Code_Invalid_Param : Error_Code_Success};

    if (Error_Code_Success == ret)
    {
        Json j;
        DVSModeConf conf{0};
        std::string factory, name, ip, port, user, passwd, timestamp, out;

        if (Error_Code_Success == j.parse(json.c_str()))
        {
            j.get("factory", factory);
            j.get("name", name);
            j.get("ip", ip);
            j.get("port", port);
            j.get("user", user);
            j.get("passwd", passwd);
            j.get("timestamp", timestamp);

            conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_NONE;
            if (!factory.compare("0"))
            {
                conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_HK;
            }
            else if (!factory.compare("1"))
            {
                conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_DH;
            }
            conf.model = DVSModelType::DVS_MODEL_TYPE_IPC;
            //设备ID从1开始原子计数
            conf.id = ++deviceNumber;
            XMem().copy(name.c_str(), name.length(), conf.name, 128);
            XMem().copy(user.c_str(), user.length(), conf.user, 128);
            XMem().copy(passwd.c_str(), passwd.length(), conf.passwd, 64);
            XMem().copy(ip.c_str(), ip.length(), conf.ip, 128);
            conf.port = atoi(port.c_str());
            
            int ret{DVSNode::addConf(conf)};

            if (Error_Code_Success == ret)
            {
                ret = DVSNode::run(conf.id);
                if (Error_Code_Success == ret)
                {
                    conf = DVSNode::queryConf(conf.id);
                }
                else
                {
                    DVSNode::removeConf(conf.id);
                }
            }
        }
        else
        {
            ret = Error_Code_Operate_Failure;
        }

        j.reset();
        j.add("error", (boost::format("%d") % ret).str());
        j.add("id", (boost::format("%d") % conf.id).str());
        j.add("channels", (boost::format("%d") % conf.channels).str());
        j.add("timestamp", (boost::format("%lld") % timestamp).str());
        j.serialize(out);
        const std::string url{
            (boost::format("config://%s?command=add&data=%s") % from % out).str()};
        XMQNode::send(modeconf.id, url.c_str(), url.length());

        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=%d&log=Add new device [ %d_%s_%s_%u_%s_%s ] result [ %d ].") 
                % logid % (ret ? 1 : 0) % conf.id % conf.name % conf.ip % conf.port % conf.user % conf.passwd % ret).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }
    
    return ret;
}

int DvsHostServer::remove(const std::string& from, const std::string& json)
{
    int ret{from.empty() || json.empty() ? Error_Code_Invalid_Param : Error_Code_Success};

    if (Error_Code_Success == ret)
    {
        Json j;
        std::string id, timestamp, out;

        if (Error_Code_Success == j.parse(json.c_str()))
        {
            j.get("id", id);
            j.get("timestamp", timestamp);

            int ret{DVSNode::removeConf(atoi(id.c_str()))};
            j.reset();
            j.add("error", (boost::format("%d") % ret).str());
            j.add("timestamp", (boost::format("%lld") % timestamp).str());
            j.serialize(out);
            const std::string url{
                    (boost::format("config://%s?command=remove&data=%s") % from % out).str()};
            XMQNode::send(modeconf.id, url.c_str(), url.length());
            
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=%d&log=Remove device [ %s ] result [ %d ].") 
                    % logid % (ret ? 1 : 0) % id.c_str() % ret).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
    }

    return ret;
}

int DvsHostServer::query(const std::string& from, const std::string& json)
{
    int ret{from.empty() || json.empty() ? Error_Code_Invalid_Param : Error_Code_Success};

    if (Error_Code_Success == ret)
    {
        Json j;
        std::string timestamp, out;

        if (Error_Code_Success == j.parse(json.c_str()))
        {
            j.get("timestamp", timestamp);
            j.reset();
            Items items;
            DVSModeConf* confs{ nullptr };
            uint32_t number{ 0 };
            ret = DVSNode::queryConfs(confs, number);

            if (Error_Code_Success == ret)
            {
                for (int i = 0; i != number; ++i)
                {
                    items.push_back(std::make_pair("factory", (boost::format("%s") % (int)confs[i].factory).str()));
                    items.push_back(std::make_pair("name", confs[i].name));
                    items.push_back(std::make_pair("id", (boost::format("%d") % confs[i].id).str()));
                    items.push_back(std::make_pair("ip", confs[i].ip));
                    items.push_back(std::make_pair("channels", (boost::format("%d") % confs[i].channels).str()));
                    j.add("devices", items);
                }
            }

            j.add("timestamp", timestamp);
            j.add("error", (boost::format("%d") % ret).str());
            j.serialize(out);
            std::string url{
                (boost::format("config://%s?command=query&data=%s") % from % out).str()};
            XMQNode::send(modeconf.id, url.c_str(), url.length());
        }
    }

    return ret;
}

void DvsHostServer::afterPolledRealplayDataNotification(
    const uint32_t id/* = 0*/, 
    const int32_t channel/* = 0*/, 
    const uint32_t type/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint32_t bytes/* = 0*/)
{
    const std::vector<DVSStreamSessionPtr> ss{sessions.values()};
    const std::string fidkey{ (boost::format("%u_%d") % id % channel).str() };
    uint64_t fid{ fids.at(fidkey) };
    fids.replace(fidkey, ++fid);

    for (int i = 0; i != ss.size(); ++i)
    {
        uint32_t sid{ 0 }, did{ 0 }, cid{ 0 };
        uint64_t _fid{ 0 };
        ss[i]->getIDs(sid, did, cid, _fid);

        if(0 < sid && did == id && cid == channel)
        {
            const uint32_t totalBytes{bytes + 32};
            char* frameData{ new(std::nothrow) char[totalBytes] };
            *((uint32_t*)frameData) = 0xFF050301;
            *((uint32_t*)(frameData + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_VIDEO;
            *((uint32_t*)(frameData + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_PS;
            *((uint32_t*)(frameData + 12)) = bytes;
            *((uint64_t*)(frameData + 16)) = fid;
            *((uint64_t*)(frameData + 24)) = 0;
            XMem().copy(data, bytes, frameData + 32, bytes);
            ASIONode::send(sid, frameData, totalBytes);
            boost::checked_array_delete(frameData);
        }
    }
}
