#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "dvs_host_server.h"

DvsHostServer::DvsHostServer(FileLog& log)
    : XMQNode(), ASIONode(), DVSNode(), fileLog{log}, deviceNumber{0}
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
    const char* name/* = nullptr*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Fetch forward data = [ %s ] from xmq host server.", 
        msg.c_str());

    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("config"))
        {
            processDvsControlMessage(requestUrl);
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Parsed unknown data = [ %s ] from xmq host server.",  
                msg.c_str());
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed forward data = [ %s ] from xmq host server failed, result = [ %d ].",  
            msg.c_str(), 
            ret);
    }
}

uint32_t DvsHostServer::afterFetchAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/,  
    const int32_t e/* = 0*/)
{
    // if (so && !e)
    // {
    //     const std::string sid{Uuid().createNew()};

    //     if(!sid.empty())
    //     {
    //         // TcpSessionPtr ptr{boost::make_shared<DvsHostSession>(*this, sid)};

    //         // if(ptr && 
    //         //     Error_Code_Success == ptr->createNew(session) &&
    //         //     Error_Code_Success == ptr->receive())
    //         // {
    //         //     sessions.add(sid, ptr);
    //         // }
    //     }
    // }

    return 0;
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

}

void DvsHostServer::afterPolledSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{}

void DvsHostServer::processDvsControlMessage(Url& requestUrl)
{
    const std::vector<ParamItem> parameters{requestUrl.getParameters()};
    const std::string host{requestUrl.getHost()};
    std::string command, from, ip, port, user, passwd, id, name;

    for(int i = 0; i != parameters.size(); ++i)
    {
        if (!parameters[i].key.compare("command"))
        {
            command = parameters[i].value;
        }
        else if (!parameters[i].key.compare("from"))
        {
            from = parameters[i].value;
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

        XMQNode::send(0xB1, nullptr, url.c_str(), url.length());
    }
    else if (!command.compare("add"))
    {
        DVSModeConf conf{0};
        XMem().copy(conf.name, 128, (char*)user.c_str(), user.length());
        XMem().copy(conf.passwd, 64, (char*)passwd.c_str(), passwd.length());
        XMem().copy(conf.ip, 128, (char*)ip.c_str(), ip.length());
        conf.port = atoi(port.c_str());
        conf.id = ++deviceNumber;
        conf.factory = DVSFactoryType::DVS_FACTORY_TYPE_HK;
        conf.model = DVSModelType::DVS_MODEL_TYPE_IPC;
        int ret{DVSNode::addConf(conf)};

        if (Error_Code_Success == ret)
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Add new device successfully with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ].", 
                deviceNumber, name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str());

            ret = DVSNode::run(conf.id);
            if (Error_Code_Success == ret)
            {
                conf = DVSNode::queryConf(conf.id);
            }

            const std::string url{
                (boost::format("config://%s?from=%s&command=add&error=%d&dvs=%s_%s_%d_%s") 
                % from % host % ret % conf.id % ip % conf.channels % name).str()};
                
            XMQNode::send(0xB1, nullptr, url.c_str(), url.length());
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Add new device failed with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ], result = [ %d ].", 
                deviceNumber, name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str(), ret);

            const std::string url{
                (boost::format("config://%s?from=%s&command=add&error=%d") % from % host % ret).str()};
                
            XMQNode::send(0xB1, nullptr, url.c_str(), url.length());
        }
    }
    else if (!command.compare("remove"))
    {
        // int ret{dvsHostMan.removeDevice(id)};
        // std::string url;

        // if (Error_Code_Success == ret)
        // {
        //     fileLog.write(
        //         SeverityLevel::SEVERITY_LEVEL_INFO, "Remove device successfully with uuid = [ %s ].", id.c_str());
        // }
        // else
        // {
        //     fileLog.write(
        //         SeverityLevel::SEVERITY_LEVEL_WARNING, 
        //         "Remove device failed with uuid = [ %s ] result = [ %d ].", id.c_str(), ret);
        // }

        // url.append(
        //     (boost::format("config://%s?from=%s&command=remove&error=%d&id=%s") % from % host % ret % id).str());
        // XMQNode::send(0xB1, nullptr, url.c_str(), url.length());
    }
}

void DvsHostServer::afterPolledRealplayDataNotification(
    const uint32_t id/* = 0*/, 
    const int32_t channel/* = 0*/, 
    const uint32_t type/* = 0*/, 
    const uint8_t* data/* = nullptr*/, 
    const uint32_t bytes/* = 0*/)
{}
