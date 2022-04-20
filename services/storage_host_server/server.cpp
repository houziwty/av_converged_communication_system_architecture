#include "boost/format.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), Libfdfs(), Libasio(), log{flog}, 
    xid{0}, logid{std::string(StorageHostID).append("_log")}
{}

Server::~Server()
{
    Libfdfs::destroy();
}

int Server::run(
    const XMQNodeConf& conf, 
    const char* fdfsconf/* = nullptr*/)
{
    int ret{Libxmq::addNode(conf)};

    if(Error_Code_Success == ret)
    {
        xid = conf.id;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Connect to xmq server [ %s : %u ] successfully.", conf.ip, conf.port);

        //Init FastDFS connection.
        ret = Libfdfs::init(fdfsconf);
        if (Error_Code_Success == ret)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run fast dfs server successfully.");

            //Open stream download server.
            ASIOModeConf ioconf;
            ioconf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            ioconf.port = 60927;
            ioconf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
            ret = Libasio::addConf(ioconf);

            if (Error_Code_Success == ret)
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "Run storage streaming server [ %u ] successfully.", ioconf.port);
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_ERROR, 
                    "Run storage streaming server [ %u ] failed, reuslt = [ %d ].", ioconf.port, ret);
            }
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Run fast dfs server failed, result = [ %d ].", ret);
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
            "info://%s?command=add&severity=0&log=Run storage host server name [ %s ], xmq_addr [ %s ], xmq_port [ %d ], type [ %d ].") 
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

            ret = Libfdfs::destroy();
            if (Error_Code_Success == ret)
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "Stop fast dfs server successfully.");
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_ERROR, 
                    "Stop fast dfs server failed, result = [ %d ].", ret);
            }
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

void Server::afterFetchOnlineStatusNotification(const bool online)
{
	log.write(
		online ? SeverityLevel::SEVERITY_LEVEL_INFO : SeverityLevel::SEVERITY_LEVEL_WARNING,
		"Fetch message of online status [ %s ] notification.",
		online ? "online" : "offline");
}

void Server::afterFetchServiceCapabilitiesNotification(
    const char** names/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{
    std::string text;

    for (int i = 0; i != number; ++i)
    {
        text += ("[ " + std::string(names[i]) + " ]");
    }

	log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
		"Fetch response message of service table [ %s ] notification.",
        text.c_str());
}

void Server::afterPolledXMQDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const char* name/* = nullptr*/)
{
    Url requestUrl;
    int ret{requestUrl.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.proto().compare("info"))
        {
            std::string command, severity, begin, end, text, date, timestamp;
            const std::vector<Parameter> parameters{requestUrl.parameters()};
            for(int i = 0; i != parameters.size(); ++i)
            {
                if (!parameters[i].key.compare("command"))
                {
                    command = parameters[i].value;
                }
                else if (!parameters[i].key.compare("severity"))
                {
                    severity = parameters[i].value;
                }
                else if (!parameters[i].key.compare("log"))
                {
                    text = parameters[i].value;
                }
                else if (!parameters[i].key.compare("begin"))
                {
                    begin = parameters[i].value;
                }
                else if (!parameters[i].key.compare("end"))
                {
                    end = parameters[i].value;
                }
				else if (!parameters[i].key.compare("date"))
				{
					date = parameters[i].value;
				}
				else if (!parameters[i].key.compare("timestamp"))
				{
                    timestamp = parameters[i].value;
				}
            }

            if(!command.compare("add"))
            {
            }
            else if(!command.compare("query"))
            {
            }
            else if(!command.compare("fetch"))
            {
            }
        }
    }
}

uint32_t Server::afterFetchIOAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const int32_t e/* = 0*/)
{
    return 0;
}

uint32_t Server::afterFetchIOConnectedEventNotification(const int32_t e/* = 0*/)
{
    return 0;
}

void Server::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}
