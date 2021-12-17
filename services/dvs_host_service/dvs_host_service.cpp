#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "dvs_host_session.h"
#include "dvs_host_service.h"

DvsHostService::DvsHostService(FileLog& log) 
    : LibXmqHostClient(), TcpServer(), fileLog{log}
{}

DvsHostService::~DvsHostService()
{}

int DvsHostService::start(
    const std::string name, 
    const std::string ip, 
    const unsigned short port/* = 0*/)
{
    int ret{LibXmqHostClient::registerXmqHostClient(name, ip, port)};

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Start xmq host client successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Start xmq host client failed with name = [ %s ] ip = [ %s ] port = [ %d ], result = [ %d ].", 
            name.c_str(), ip.c_str(), port, ret);
    }
    
    return ret;
}

int DvsHostService::stop()
{
    unregisterXmqHostClient();
    sessions.clear();    
    return Error_Code_Success;
}

int DvsHostService::startXMS(const unsigned short port/* = 0*/)
{
    int ret{TcpServer::start(port)};

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Start XMS service successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Start XMS service failed, result = [ %d ] port = [ %d ].", 
            ret, port);
    }

    return ret;
}

int DvsHostService::stopXMS()
{
    return TcpServer::stop();
}

void DvsHostService::removeExpiredSession(const std::string sid)
{
    sessions.remove(sid);
}

void DvsHostService::fetchXmqHostClientOnlineStatusNotification(bool online)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch dvs host service online status = [ %d ].", online);
}

void DvsHostService::fetchXmqHostServiceCapabilitiesNotification(const std::vector<std::string> services)
{
    const int number{static_cast<int>(services.size())};
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq host service capabilities size = [ %d ].", number);

    for(int i = 0; i != number; ++i)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", services[i].c_str());
    }
}

void DvsHostService::fetchXmqHostClientReceivedDataNotification(const std::string data)
{
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Fetch forward data = [ %s ] from xmq host service.", 
        data.c_str());

    Url requestUrl;
    int ret{requestUrl.parse(data)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("dvs"))
        {
            processDvsControlMessage(requestUrl);
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Parsed unknown data = [ %s ] from xmq host service.",  
                data.c_str());
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed forward data = [ %s ] from xmq host service failed, result = [ %d ].",  
            data.c_str(), 
            ret);
    }
}

void DvsHostService::fetchAcceptedEventNotification(SessionPtr session, const int e/* = 0*/)
{
    if (session && !e)
    {
        const std::string sid{Uuid().createNew()};

        if(!sid.empty())
        {
            TcpSessionPtr ptr{boost::make_shared<DvsHostSession>(session, *this, sid)};

            if(ptr && 
                Error_Code_Success == ptr->createNew() &&
                Error_Code_Success == ptr->receive())
            {
                sessions.add(sid, ptr);
            }
        }
    }
}

void DvsHostService::processDvsControlMessage(Url& requestUrl)
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
            (boost::format("dvs://%s?from=%s&command=query") % from % host).str()};
        const std::vector<DeviceInfo> infos{dvsHostMan.queryDeviceInfos()};
        for (int i = 0; i != infos.size(); ++i)
        {
            std::string dvs{
                (boost::format("&dvs=%s_%s_%d_%s") 
                % infos[i].uuid.c_str() 
                % infos[i].ip.c_str() 
                % static_cast<int>(infos[i].cameras.size()) 
                % infos[i].name.c_str()).str()};
            url.append(dvs);
        }
        LibXmqHostClient::send(url);
    }
    else if (!command.compare("add"))
    {
        const std::string uuid{Uuid().createNew()};
        int ret{
            dvsHostMan.addDevice(
                uuid, 
                name, 
                user, 
                passwd, 
                ip, 
                static_cast<unsigned short>(atoi(port.c_str())), 
                FactoryType::FACTORY_TYPE_HK)};

        if (Error_Code_Success == ret)
        {
             fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Add new device successfully with uuid = [ %s ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ].", 
                uuid.c_str(), name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str());

            const CameraPtrs cameras{dvsHostMan.queryCameraPtrs(uuid)};
            const std::string url{
                (boost::format("dvs://%s?from=%s&command=add&error=%d&dvs=%s_%s_%d_%s") 
                % from % host % ret % uuid % ip % static_cast<int>(cameras.size()) % name).str()};
            LibXmqHostClient::send(url);
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Add new device failed with uuid = [ %s ] name = [ %s ] ip = [ %s ] port = [ %s ], user = [ %s ], passwd = [ %s ], result = [ %d ].", 
                uuid.c_str(), name.c_str(), ip.c_str(), port.c_str(), user.c_str(), passwd.c_str(), ret);

            const std::string url{
                (boost::format("dvs://%s?from=%s&command=add&error=%d") % from % host % ret).str()};
            LibXmqHostClient::send(url);
        }
    }
    else if (!command.compare("remove"))
    {
        int ret{dvsHostMan.removeDevice(id)};
        std::string url;

        if (Error_Code_Success == ret)
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, "Remove device successfully with uuid = [ %s ].", id.c_str());
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Remove device failed with uuid = [ %s ] result = [ %d ].", id.c_str(), ret);
        }

        url.append(
            (boost::format("dvs://%s?from=%s&command=remove&error=%d&id=%s") % from % host % ret % id).str());
        LibXmqHostClient::send(url);
    }
}
