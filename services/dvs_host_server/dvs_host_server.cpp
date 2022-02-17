#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "av_pkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "dvs_host_server.h"

DvsHostServer::DvsHostServer(FileLog& log)
    : XMQNode(), ASIONode(), DVSNode(), fileLog{log}, deviceNumber{0}, streamNumber{0}
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
            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = 60820;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;

            ret = ASIONode::addConf(conf);
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

void DvsHostServer::removeExpiredSession(const std::string sid)
{
//    sessions.remove(sid);
}

void DvsHostServer::afterFetchOnlineStatusNotification(const bool online)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch dvs host service online status = [ %d ].", online);
}

void DvsHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq host service capabilities size = [ %d ].", number);

    for(int i = 0; i != number; ++i)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", infos[i].name);
    }
}

void DvsHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Polled forward data = [ %s ] from server = [ %s ].", 
        msg.c_str(), 
        from);

    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("config"))
        {
            processDvsControlMessage(from, requestUrl);
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Parsed unknown data = [ %s ] from server = [ %s ].",  
                msg.c_str(), 
                from);
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed forward data = [ %s ] from server = [ %s ] failed, result = [ %d ].",  
            msg.c_str(), 
            from, 
            ret);
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
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Fetch stream session id = [ %d ] connect from ip = [ %s ], port = [ %d ] successfully, result = [ %d ].",  
            streamNumber, ip, port, e);
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Fetch stream session id = [ %d ] connect from ip = [ %s ], port = [ %d ] failed, result = [ %d ].",  
            streamNumber, ip, port, e);
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
            sess = boost::make_shared<DvsStreamSession>(fileLog, id);
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
        int ret{ASIONode::removeConf(id)};

        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Fetch stream session id = [ %d ] exception code = [ %d ], and remove it result = [ %d ].",  
            id, e, ret);
    }
}

void DvsHostServer::afterPolledSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{}

void DvsHostServer::processDvsControlMessage(const std::string from, Url& requestUrl)
{
    const std::vector<ParamItem> parameters{requestUrl.getParameters()};
    const std::string host{requestUrl.getHost()};
    std::string command, ip, port, user, passwd, id, name;

    for(int i = 0; i != parameters.size(); ++i)
    {
        if (!parameters[i].key.compare("command"))
        {
            command = parameters[i].value;
        }
        else if (!parameters[i].key.compare("ip"))
        {
            ip = parameters[i].value;
        }
        else if (!parameters[i].key.compare("port"))
        {
            port = parameters[i].value;
        }
        else if (!parameters[i].key.compare("user"))
        {
            user = parameters[i].value;
        }
        else if (!parameters[i].key.compare("passwd"))
        {
            passwd = parameters[i].value;
        }
        else if (!parameters[i].key.compare("id"))
        {
            id = parameters[i].value;
        }
        else if (!parameters[i].key.compare("name"))
        {
            name = parameters[i].value;
        }
    }

    if (!command.compare("query"))
    {
        std::string url{
            (boost::format("config://%s?command=query") % from).str()};
        std::vector<DVSModeConf> confs;
        DVSNode::queryConf(confs);

        for (int i = 0; i != confs.size(); ++i)
        {
            const std::string dvs{
                (boost::format("&dvs=%d_%s_%d_%s") % confs[i].id % confs[i].ip % confs[i].channels % confs[i].name).str()};
            url.append(dvs);
        }

        int ret{XMQNode::send(0xB1, url.c_str(), url.length())};

        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Query device information successfully from service = [ %s ], result = [ %d ].", 
            from.c_str(), ret);
    }
    else if (!command.compare("add"))
    {
        DVSModeConf conf{0};
        XMem().copy(name.c_str(), name.length(), conf.name, 128);
        XMem().copy(user.c_str(), user.length(), conf.user, 128);
        XMem().copy(passwd.c_str(), passwd.length(), conf.passwd, 64);
        XMem().copy(ip.c_str(), ip.length(), conf.ip, 128);
        conf.port = atoi(port.c_str());
        conf.id = ++deviceNumber;
        conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_HK;
        conf.model = DVSModelType::DVS_MODEL_TYPE_IPC;
        int ret{DVSNode::addConf(conf)};

        if (Error_Code_Success == ret)
        {
            ret = DVSNode::run(conf.id);
            if (Error_Code_Success == ret)
            {
                conf = DVSNode::queryConf(conf.id);
            }

            const std::string url{
                (boost::format("config://%s?command=add&error=%d&dvs=%s_%s_%d_%s") 
                % from % ret % conf.id % ip % conf.channels % name).str()};
            ret = XMQNode::send(0xB1, url.c_str(), url.length());

            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Add new device successfully with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ], result = [ %d ].", 
                deviceNumber, name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str(), ret);
        }
        else
        {
            const std::string url{
                (boost::format("config://%s?command=add&error=%d") % from % ret).str()};
            XMQNode::send(0xB1, url.c_str(), url.length());

            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Add new device failed with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ], result = [ %d ].", 
                deviceNumber, name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str(), ret);
        }
    }
    else if (!command.compare("remove"))
    {
        int ret{DVSNode::removeConf(atoi(id.c_str()))};
        const std::string msg{
                (boost::format("config://%s?command=remove&error=%d&id=%s") % from % ret % id).str()};
        XMQNode::send(0xB1, msg.c_str(), msg.length());

        if (Error_Code_Success == ret)
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Remove device successfully with id = [ %s ].", 
                id.c_str());
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Remove device failed with uuid = [ %s ] result = [ %d ].",
                id.c_str(), 
                ret);
        }
    }
}

void DvsHostServer::afterPolledRealplayDataNotification(
    const uint32_t id/* = 0*/, 
    const int32_t channel/* = 0*/, 
    const uint32_t type/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint32_t bytes/* = 0*/)
{
    const std::vector<DVSStreamSessionPtr> ss{sessions.values()};

    for (int i = 0; i != ss.size(); ++i)
    {
        uint32_t sid{0}, did{0}, cid{0};
        ss[i]->getIDs(sid, did, cid);

        if(0 < sid && did == id && cid == channel)
        {
            const uint32_t totalBytes{bytes + 32};
            char* frameData{ new(std::nothrow) char[totalBytes] };
            *((uint32_t*)frameData) = 0xFF050301;
            *((uint32_t*)(frameData + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_HK_PS;
            *((uint32_t*)(frameData + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_VIDEO;
            *((uint32_t*)(frameData + 12)) = bytes;
            *((uint64_t*)(frameData + 16)) = 0;
            *((uint64_t*)(frameData + 24)) = 0;
            XMem().copy(data, bytes, frameData + 32, bytes);
            ASIONode::send(sid, frameData, totalBytes);
            boost::checked_array_delete(frameData);
        }
    }
}
