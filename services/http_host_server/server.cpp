#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), Libasio(), Libhttp(), log{flog}, xid{0}, 
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

        //Open http server.
        ASIOModeConf http;
        http.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        http.port = 18080;
        http.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
        ret = Libasio::addConf(http);

        if (Error_Code_Success == ret)
        {
            httpValid = true;
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run http server [ %u ] successfully.", http.port);
        }
        else
        {
             log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Run http server [ %u ] failed, reuslt = [ %d ].", http.port, ret);
        }

        //Open https server.
        ASIOModeConf https;
        https.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
        https.port = 18443;
        https.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_LISTEN;
        ret = Libasio::addConf(https);

        if (Error_Code_Success == ret)
        {
            httpValid = true;
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run https server [ %u ] successfully.", https.port);
        }
        else
        {
             log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Run https server [ %u ] failed, reuslt = [ %d ].", https.port, ret);
        }

        if (httpValid)
        {
            ret = loadHttpAPIList();
        }
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

uint32_t Server::afterFetchIOAcceptedEventNotification(
    const char* remoteIP/* = nullptr*/, 
    const uint16_t remotePort/* = 0*/, 
    const uint16_t localPort/* = 0*/, 
    const int32_t e/* = 0*/)
{
    uint32_t sid{0};

    if (!e)
    {
        sid = ++sessionIDNumber;
        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Fetch remote connect from [ %s:%u ] successfully, session ID = [ %u ]", 
            remoteIP, remotePort, sid);
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Fetch remote connect from [ %s:%u ] failed, error = [ %d ]", 
            remoteIP, remotePort, e);
    }

    if (0 < sid)
    {
        Libhttp::addSession(sid);
    }
    
    return sid;
}

uint32_t Server::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    return 0;
}

void Server::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    const std::string temp{ (const char*)data, bytes };
    log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "\r\n%s", temp.c_str());

    if (e)
    {
        Libhttp::removeSession(id);
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Capture data read error [ %d ] on session [ %u ]", e, id);
    }
    else if (data && 0 < bytes && 0 < id)
    {
        Libhttp::input(id, data, bytes);
    }
}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{}

void Server::afterFetchHttpResponseNotification(
    const uint32_t id/* = 0*/, 
    const char* response/* = nullptr*/, 
    const bool close/* = false*/)
{
    if (0 < id && response)
    {
        const std::string responseStr{response};
        //发送应答
        int ret{Libasio::send(id, response, responseStr.length())};

        if (Error_Code_Success == ret)
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Send response on session ID [ %u ] successfully\r\n%s", id, response);
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "Send response on session ID [ %u ] successfully, error = [ %d ]\r\n%s", id, ret, response);
        }

        //根据会话关闭标识控制会话是否关闭
        if (close)
        {
            ret = Libasio::removeConf(id);

            if (Error_Code_Success == ret)
            {
                ret = Libhttp::removeSession(id);
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "Remove HTTP session ID [ %u ] successfully.", id);
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, 
                    "Remove HTTP session ID [ %u ], error = [ %d ]", id, ret);
            }
        }
    }
}

void Server::afterFetchHttpRequestNotification(
    const uint32_t id, const char* url, int& e, char*& body, char*& type)
{
    if (0 < id && url)
    {
        //通过URL中是否带有参数来区分GET和POST请求
        //这个不属于HTTP协议范围内的解析
        //由应用端自行解析
        const std::string request_url{url};
        const std::size_t pos{ request_url.find_first_of('?') };
        const std::string command{ request_url.substr(0, pos) };
        const std::string parameters{ 
            std::string::npos != pos ? 
            request_url.substr(pos + 1, request_url.length() - pos - 1) : 
            ""};

        std::pair<AfterFetchHttpRequestCallback, std::string> callback{callbacks.at(command)};
        if (callback.first)
        {
            callback.first(parameters.c_str(), e, body, type);
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Fetch unsupport HTTP request [ %s ] on session [ %u ]", command, id);
        }
    }
}

void Server::getAPIList(const char* params, int& e, char*& body, char*& type)
{
    e = 200;
    boost::json::object o;
    
    for (std::unordered_map<std::string, std::pair<AfterFetchHttpRequestCallback, std::string>>::iterator it = callbacks.begin(); 
        it != callbacks.end();++it)
    {
        o[it->first] = it->second.second;
    }

    const std::string out{boost::json::serialize(o)};
    const std::size_t len{out.length()};
    if (!out.empty())
    {
        body = reinterpret_cast<char*>(XMem().alloc(out.c_str(), len));
        const std::string temp{"text/json"};
        type = reinterpret_cast<char*>(XMem().alloc(temp.c_str(), temp.length()));
    }
}

void Server::addDevice(const char* params, int& e, char*& body, char*& type)
{
    
}

void Server::removeDevice(const char* params, int& e, char*& body, char*& type)
{

}

int Server::loadHttpAPIList()
{
    //获取API列表
    callbacks.emplace(
        "/api/v1/getapilist", 
        std::make_pair(boost::bind(&Server::getAPIList, this, _1, _2, _3, _4), "获取API接口"));
    ////新增DVS设备
    //invokers.add(
    //    "/api/v1/device/add", 
    //    boost::bind(&Server::afterFetchAPIInvokeAddDevice, this, _1, _2, _3, _4));
    ////删除DVS设备
    //invokers.add(
    //    "/api/v1/device/remove", 
    //    boost::bind(&Server::afterFetchAPIInvokeRemoveDevice, this, _1, _2, _3, _4));
    
    return Error_Code_Success;
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

        if (!command.compare("mec.dvs.add"))
        {
            std::string id;
            auto factory{ o.at("factory").as_string() },
                name{ o.at("name").as_string() },
                ip{ o.at("ip").as_string() },
                port{ o.at("port").as_string() },
                user{ o.at("user").as_string() },
                passwd{ o.at("passwd").as_string() },
                timestamp{ o.at("timestamp").as_string() },
                sn{o.at("sn").as_string()};
            if (o.contains("id"))
            {
                id = o.at("id").as_string().c_str();
            }
        }
        else if (!command.compare("mec.dvs.remove"))
        {
        }
    }
}
