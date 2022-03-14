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

DvsHostServer::DvsHostServer(const XMQModeConf& conf)
    : XMQNode(), ASIONode(), DVSNode(), modeconf{conf}, deviceNumber{0}, streamNumber{0}
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
            logid.append(DVSHostID).append("_log");
            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = 60820;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            ASIONode::addConf(conf);

            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Run dvs host server name = [ %s ], xmq_addr = [ %s ], xmq_port = [ %d ], type = [ %d ].") 
                    % logid % modeconf.name % modeconf.ip % modeconf.port % static_cast<int>(modeconf.type)).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
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
}

void DvsHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
}

void DvsHostServer::afterPolledDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* from/* = nullptr*/)
{
    const std::string msg{reinterpret_cast<const char*>(data), bytes};
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
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=1&log=Parsed unknown url = [ %s ] from server = [ %s ].") 
                    % logid % msg % from).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
    }
    else
    {
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=2&log=Parsed polled url = [ %s ] from server = [ %s ] failed, result = [ %d ].") 
                % logid % msg % from % ret).str()};
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
                "info://%s?command=add&severity=0&log=Fetch stream session id = [ %d ] connect from ip = [ %s ], port = [ %d ] successfully, result = [ %d ].") 
                % logid % streamNumber % ip % e).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
    }
    else
    {
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=1&log=Fetch stream session id = [ %d ] connect from ip = [ %s ], port = [ %d ] successfully, result = [ %d ].") 
                % logid % streamNumber % ip % e).str()};
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
        int ret{ASIONode::removeConf(id)};
        const std::string log{
            (boost::format(
                "info://%s?command=add&severity=1&log=Fetch stream session id = [ %d ] exception code = [ %d ], and remove it result = [ %d ].") 
                % logid % id % e % ret).str()};
        XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
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
        DVSModeConf* confs{ nullptr };
        uint32_t number{ 0 };

        if (Error_Code_Success == DVSNode::queryConfs(confs, number))
        {
			for (int i = 0; i != number; ++i)
			{
				const std::string dvs{
					(boost::format("&dvs=%d_%s_%d_%s") % confs[i].id % confs[i].ip % confs[i].channels % confs[i].name).str() };
				url.append(dvs);
			}
        }
        
        XMQNode::send(modeconf.id, url.c_str(), url.length());
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
            else
            {
                DVSNode::removeConf(conf.id);
            }

            const std::string url{
                (boost::format("config://%s?command=add&error=%d&dvs=%s_%s_%d_%s") 
                % from % ret % conf.id % ip % conf.channels % name).str()};
            int send_ret{ XMQNode::send(modeconf.id, url.c_str(), url.length()) };

            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Add new device with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ] error = [ %d ], result = [ %d ].") 
                    % logid % deviceNumber % name % ip % port % user % passwd % ret % send_ret).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
        else
        {
            const std::string url{
                (boost::format("config://%s?command=add&error=%d") % from % ret).str()};
            XMQNode::send(modeconf.id, url.c_str(), url.length());

            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=1&log=Add new device failed with id = [ %d ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ], result = [ %d ].") 
                    % logid % deviceNumber % name % ip % port % user % passwd % ret).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
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
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Remove device successfully with id = [ %s ].") 
                    % logid % id).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
        }
        else
        {
            const std::string log{
                (boost::format(
                    "info://%s?command=add&severity=0&log=Remove device failed with uuid = [ %s ] result = [ %d ].") 
                    % logid % id % ret).str()};
            XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
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
