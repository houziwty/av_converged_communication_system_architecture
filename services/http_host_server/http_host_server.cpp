#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "media_host_server.h"

MediaHostServer::MediaHostServer(FileLog& log)
    : Libasio(), flog{log}, sessionIDNumber{0}
{}

MediaHostServer::~MediaHostServer()
{}

int MediaHostServer::addPort(const std::string name, const uint16_t port/* = 0*/)
{
    int ret{!name.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        protos.add(port, name);
    }
    
    return ret;
}

int MediaHostServer::loadApi()
{
    //获取API列表
    apis.add(
        "/api/v1/getapilist", 
        boost::bind(&MediaHostServer::afterFetchApiEventGetApiList, this, _1, _2, _3, _4));
    //新增DVS设备
    apis.add(
        "/api/v1/device/add", 
        boost::bind(&MediaHostServer::afterFetchApiEventCreateNewDevice, this, _1, _2, _3, _4));
    //删除DVS设备
    apis.add(
        "/api/v1/device/remove", 
        boost::bind(&MediaHostServer::afterFetchApiEventRemoveDevice, this, _1, _2, _3, _4));
    
    return Error_Code_Success;
}

uint32_t MediaHostServer::afterFetchIOAcceptedEventNotification(
    const char* remoteIP/* = nullptr*/, 
    const uint16_t remotePort/* = 0*/, 
    const uint16_t localPort/* = 0*/, 
    const int32_t e/* = 0*/)
{
    uint32_t currentSID{0};

    if (!e)
    {
        currentSID = ++sessionIDNumber;
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "接收[ %s:%u ]连接本地端口[ %u ]成功,会话ID=%u", 
            remoteIP, remotePort, localPort, currentSID);
    }
    else
    {
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "接收[ %s:%u ]连接本地端口[ %u ]失败,错误码=%d", 
            remoteIP, remotePort, localPort, e);
    }

    if (0 < currentSID)
    {
        createNewSession(localPort, currentSID);
    }
    
    return currentSID;
}

uint32_t MediaHostServer::afterFetchIOConnectedEventNotification(
    const int32_t e/* = 0*/, 
    void* user/* = nullptr*/)
{
    return 0;
}

void MediaHostServer::afterPolledIOReadDataNotification(
    const uint32_t id/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{
    flog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "\r\n%s", (const char*)data);

    if (e)
    {
        sessions.remove(id);
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "捕获会话[ %u ]数据读取异常,错误码=[ %d ]", id, e);
    }
    else if (data && 0 < bytes && 0 < id)
    {
        const std::string name{sessions.at(id)};
        
        if (!name.compare("http") || !name.compare("https"))
        {
            Libhttp::input(id, data, bytes);
        }
        else if (!name.compare("rtsp") || !name.compare("rtsps"))
        {
            /* code */
        }
        else if (!name.compare("rtmp") || !name.compare("rtmps"))
        {
            /* code */
        }
        else if (!name.compare("rtp"))
        {
            /* code */
        }
    }
}

void MediaHostServer::afterPolledIOSendDataNotification(
    const uint32_t id/* = 0*/, 
    const uint64_t bytes/* = 0*/, 
    const int32_t e/* = 0*/)
{

}

int MediaHostServer::createNewSession(const uint16_t port/* = 0*/, const uint32_t id/* = 0*/)
{
    int ret{0 < port && 0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string name{protos.at(port)};
        
        if (!name.compare("http") || !name.compare("https"))
        {
            ret = Libhttp::addSession(id);
        }
        else if (!name.compare("rtsp") || !name.compare("rtsps"))
        {
            /* code */
        }
        else if (!name.compare("rtmp") || !name.compare("rtmps"))
        {
            /* code */
        }
        else if (!name.compare("rtp"))
        {
            /* code */
        }

        if (Error_Code_Success == ret)
        {
            sessions.add(id, name);
            flog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "创建[ %s ]会话[ %u ]", name.c_str(), id);
        }
    }
    else
    {
        flog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "创建会话[ %u:%u ]失败", port, id);
    }

    return ret;
}

void MediaHostServer::afterFetchHttpResponseNotification(
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
            flog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "发送HTTP会话[ %u ]应答数据成功\r\n%s", id, response);
        }
        else
        {
            flog.write(
                SeverityLevel::SEVERITY_LEVEL_ERROR, 
                "发送HTTP会话[ %u ]应答数据失败,错误码=[ %d ]\r\n%s", id, ret, response);
        }

        //根据会话关闭标识控制会话是否关闭
        if (close)
        {
            ret = Libasio::removeConf(id);

            if (Error_Code_Success == ret)
            {
                ret = Libhttp::removeSession(id);
                flog.write(
                    SeverityLevel::SEVERITY_LEVEL_INFO, 
                    "删除HTTP会话[ %u ]成功", id);
            }
            else
            {
                flog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, 
                    "删除HTTP会话[ %u ]失败,错误码=[ %d ]", id, ret);
            }
        }
    }
}

void MediaHostServer::afterFetchHttpApiEventNotification(
    const uint32_t id, const char* api, int& e, char*& body, char*& type)
{
    if (0 < id && api)
    {
        const std::string temp{api};
        const std::size_t pos{temp.find_first_of('?')};
        const std::string command{temp.substr(0, pos)};
        std::string parameters;

        if (std::string::npos != pos)
        {
            parameters = temp.substr(pos + 1, temp.length() - pos - 1);
        }

        AfterFetchApiEventNotification notification{apis.at(command)};
        if (notification)
        {
            notification(!parameters.empty() ? nullptr : parameters.c_str(), e, body, type);
        }
        else
        {
            flog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "接收HTTP会话[ %u ]不支持的API请求[ %s ]", id, command);
        }
    }
}

void MediaHostServer::afterFetchApiEventGetApiList(const char* params, int& e, char*& body, char*& type)
{
    e = 200;
    const std::vector<std::string> apilist{apis.keies()};
    boost::json::object o;
    
    for (int i = 0; i != apilist.size(); ++i)
    {
        const std::string key{(boost::format("%d") % (i + 1)).str()};
        o[key] = apilist[i]; 
    }

    //  {
    //      "1":"/api/v1/getapilist",
    //      ...
    //  }
    const std::string out{boost::json::serialize(o)};
    const std::size_t len{out.length()};
    if (!out.empty())
    {
        body = reinterpret_cast<char*>(XMem().alloc(out.c_str(), len));
        const std::string temp{"text/json"};
        type = reinterpret_cast<char*>(XMem().alloc(temp.c_str(), temp.length()));
    }
}
