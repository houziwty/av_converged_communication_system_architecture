#include "boost/algorithm/string.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libavpkt.h"
using namespace module::av::stream;
#include "url/url.h"
using namespace framework::utils::data;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "upload_session.h"
#include "download_session.h"
#include "server.h"

Server::Server(FileLog& flog)
    : Libxmq(), Libfdfs(), Libasio(), log{flog}, sid{0}, 
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

int Server::send(
    const uint32_t sid/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{0 < sid && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        while(Error_Code_Success == Libasio::send(sid, data, bytes))
        {
            XTime().sleep(10);
        }

        log.write(
            SeverityLevel::SEVERITY_LEVEL_INFO,
            "Send request of realplay stream [ %d ] with [ %s ].",
            sid, (const char*)data);
    }

    return ret;
}

int Server::upload(
    const uint32_t sid/* = 0*/, 
    const void* avpkt/* = nullptr*/, 
    const bool append/* = false*/)
{
    int ret{0 < sid && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        Libavpkt* pkt{reinterpret_cast<Libavpkt*>(const_cast<void*>(avpkt))};
        const char* fname{
            Libfdfs::upload(sid, pkt->data(), pkt->bytes(), append)};

        //只有新增才记录数据库
        if (append)
        {
            const std::string tm{
                boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time())};
            const std::string url{
                (boost::format("config://%s?data={\"command\":\"mec.db.record.add\",\"name\":\"%s\",\"time\":\"%s\"}") % DatabaseHostID % fname % tm).str()};
            ret = Libxmq::send(xid, url.c_str(), url.length(), nullptr);
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Update file name [ %s ] and time [ %s ] to database.", fname, tm.c_str());
        }
    }
    
    return ret;
}

int Server::download(
    const uint32_t sid/* = 0*/, 
    const char* filename/* = nullptr*/)
{
    int ret{0 < sid && filename ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        char* buffer{nullptr};
        int64_t bytes{0};
        ret = Libfdfs::download(sid, filename, buffer, &bytes);

        if (!ret && buffer && 0 < bytes)
        {
            ret = Libasio::send(sid, buffer, bytes);
            boost::checked_array_delete(buffer);
        }
    }

    return ret;
}

int Server::remove(
    const uint32_t sid/* = 0*/, 
    const char* filename/* = nullptr*/)
{
    int ret{0 < sid && filename ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = Libfdfs::remove(sid, filename);
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

void Server::afterFetchServiceCapabilitiesNotification(const char* names/* = nullptr*/)
{
	log.write(
        SeverityLevel::SEVERITY_LEVEL_INFO,
		"Fetch response message of service table [ %s ] notification.", names);
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
        const std::string text{(const char*)data};
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Parse request message [ %s ] from [ %s ] failed, result [ %d ].",
            text.c_str(), name, ret);
    }
}

uint32_t Server::afterFetchIOAcceptedEventNotification(
    const char* ip/* = nullptr*/, 
    const uint16_t port/* = 0*/, 
    const int32_t e/* = 0*/)
{
    //创建存储服务客户端连接会话
    //该会话处理存储数据发送
    uint32_t sessionid{0};

    if (!e)
    {
        sessionid = ++sid;
        SessionPtr ptr{
            boost::make_shared<DownloadSession>(*this, sessionid)};

        if (ptr && Error_Code_Success == ptr->run())
        {
            sessions.add(sessionid, ptr);
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Add new download session from [ %s : %u ] successfully.", ip, port);
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING,
                "Add new download session from [ %s : %u ] failed.", ip, port);
        }
    }
    else
    {
        log.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING,
            "Fetch connection [ %d ] from remote [ %s : %u ] failed, result = [ %d ].", sessionid, ip, port, e);
    }
    
    return sessionid;
}

uint32_t Server::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    //创建实时视频数据读取会话
    //与DVS设备接入主机连接
    uint32_t sessionid{0};

    if (!e && user)
    {
        sessionid = ++sid;
        const std::string ids{(const char*)user};
        std::vector<std::string> items;
        boost::split(items, ids, boost::is_any_of("_"));
        SessionPtr ptr{
            boost::make_shared<UploadSession>(
                *this, sessionid, (uint32_t)atoi(items[1].c_str()), (uint32_t)atoi(items[2].c_str()))};

        if (ptr && Error_Code_Success == ptr->run())
        {
            sessions.add(sessionid, ptr);
            tasks.add((uint32_t)atoi(items[0].c_str()), ptr);
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Add new upload session with ids [ %s ] successfully.", ids.c_str());
        }
        else
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING,
                "Add new upload session with ids [ %s ] failed.", ids.c_str());
        }

        boost::checked_array_delete((char*)user);
    }

    return sessionid;
}

void Server::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    if (!e)
    {
        if (0 < id && data && 0 < bytes)
        {
            SessionPtr ptr{sessions.at(id)};

            if (ptr)
            {
                ptr->input(data, bytes);
            }
        }
    }
    else
    {
        sessions.remove(id);
    }
}

void Server::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

void Server::processConfigRequest(
    const char* name/* = nullptr*/, 
    const char* req/* = nullptr*/)
{
    if (name && req)
    {
        auto o{boost::json::parse(req).as_object()};
        auto command{o.at("command").as_string()}, timestamp{o.at("timestamp").as_string()};
        std::string out;

        if (!command.compare("mec.task.set"))
        {
            auto did{o.at("camera_id").as_string()}, 
                channel{o.at("camera_channels").as_string()}, 
                tid{o.at("task_id").as_string()};
            const std::string ids{
                (boost::format("%s_%s_%s") % tid.c_str() % did.c_str() % channel.c_str()).str()};

            ASIOModeConf conf;
            conf.proto = ASIOProtoType::ASIO_PROTO_TYPE_TCP;
            conf.port = 60820;
            conf.tcp.mode = ASIOModeType::ASIO_MODE_TYPE_CONNECT;
            conf.tcp.ip = "127.0.0.1";
            conf.tcp.user = XStr().alloc(ids.c_str(), ids.length());
            Libasio::addConf(conf);

            //Don't reply
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Fetch request for creating task of realplay stream [ %s ].", ids.c_str());
        }
        else if (!command.compare("mec.task.remove"))
        {
            auto tid{o.at("task_id").as_string()};

            SessionPtr ptr{tasks.at((uint32_t)atoi(tid.c_str()))};
            if (ptr)
            {
                const uint32_t sid{ptr->id()};
                ptr->stop();
                sessions.remove(sid);
                tasks.remove(sid);

                log.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO,
                    "Fetch task id for removing realplay stream [ %s ] successfully.", tid.c_str());
            }
            else
            {
                log.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING,
                    "Fetch invalid task id for removing realplay stream [ %s ].", tid.c_str());
            }
        }
    }
}
