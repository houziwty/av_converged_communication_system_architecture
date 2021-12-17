#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libasio/service.h"
#include "libasio/connector.h"
using namespace module::network::asio;
#include "libasio/tcp/tcp_session.h"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;
#include "utils/map/unordered_map.h"
#include "xms_client_session.h"
#include "libxms_host_client.h"

using ConnectorPtr = boost::shared_ptr<Connector>;
using TcpSessionPtr = boost::shared_ptr<TcpSession>;
Service service;
UnorderedMap<const std::string, TcpSessionPtr> sessions;

LibXmsHostClient::LibXmsHostClient()
{}

LibXmsHostClient::~LibXmsHostClient()
{
    stopXmsHostClient();
}

int LibXmsHostClient::startXmsHostClient()
{
    return service.start();
}

int LibXmsHostClient::stopXmsHostClient()
{
    sessions.clear();
    return service.stop();
}

int LibXmsHostClient::connect(const std::string ip, const unsigned short port/* = 0*/)
{
    int ret{!ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ConnectorPtr ptr{
            boost::make_shared<Connector>(
                [](SessionPtr session, const int e)
                {
                    const std::string uuid{Uuid().createNew()};
                    TcpSessionPtr ptr{
                        boost::make_shared<XmsClientSession>(session, uuid)};

                    if(!uuid.empty() && ptr)
                    {
                        sessions.insert(uuid, ptr);
                        fetchXmsHostClientConnectedNotification(uuid, e);
                    }
                })};

        if (ptr)
        {
            ret = ptr->connect(service.ctx(), ip.c_str(), port);
        }   
    }
    
    return ret;
}

int LibXmsHostClient::disconnect(const std::string sid)
{
    int ret{!sid.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        sessions.remove(sid);
    }

    return ret;
}
