#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "libavpkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "server.h"
#include "upload_session.h"

UploadSession::UploadSession(
    Server& svr, 
    const uint32_t sid/* = 0*/, 
    const uint32_t did/* = 0*/, 
    const uint32_t cid/* = 0*/) 
    : Session(svr, sid), deviceid{did}, channelid{cid}, 
    thread{nullptr}, frameNumber{0}
{}

UploadSession::~UploadSession()
{}

int UploadSession::run()
{
    int ret{0 < sessionid && 0 < deviceid && 0 < channelid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        AVModeConf conf;
        conf.id = sessionid;
        conf.type = AVModeType::AV_MODE_TYPE_GRAB_PS;
        conf.hwnd = nullptr;
        conf.infos = nullptr;
        conf.callback = boost::bind(&UploadSession::afterGrabPSFrameDataNotification, this, _1, _2);
        ret = Libav::addConf(conf);

        if (Error_Code_Success == ret)
        {
            thread = ThreadPool().get_mutable_instance().createNew(
                boost::bind(
                    &UploadSession::sendRealplayRequestThread, this));
            ret = (thread ? Error_Code_Success : Error_Code_Bad_New_Thread);
        }
    }
    
    return ret;
}

int UploadSession::stop()
{
    int ret{0 < sessionid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ThreadPool().get_mutable_instance().destroy(thread);
        ret = Libav::removeConf(sessionid);
    }
    
    return ret;
}

int UploadSession::input(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        Libavpkt pkt;
        ret = pkt.input(data, bytes);

        if (Error_Code_Success == ret)
        {
            ret = Libav::input(sessionid, &pkt);
        }
    }
    
    return ret;
}

void UploadSession::sendRealplayRequestThread()
{
    const std::string req{
        (boost::format("realplay://%d?data={\"command\":\"1\",\"channel\":\"%d\",\"stream\":0") % deviceid % channelid).str()};
    const std::size_t bytes{req.length()};
    const uint64_t totalBytes{bytes + 32};
    char* frameData{ new(std::nothrow) char[totalBytes] };
    *((uint32_t*)frameData) = 0xFF050301;
    *((uint32_t*)(frameData + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_VIDEO;
    *((uint32_t*)(frameData + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_PS;
    *((uint32_t*)(frameData + 12)) = bytes;
    *((uint64_t*)(frameData + 16)) = ++sequence;
    *((uint64_t*)(frameData + 24)) = 0;
    XMem().copy(req.c_str(), bytes, frameData + 32, bytes);
    server.send(sessionid, frameData, totalBytes);
    boost::checked_array_delete(frameData);
}

void UploadSession::afterGrabPSFrameDataNotification(
    const uint32_t sid/* = 0*/, 
    const void* avpkt/* = nullptr*/)
{
    if(sessionid == sid && avpkt)
    {
        bool append{false};

        if (!frameNumber || frameNumber >= maxFrameNumber)
        {
            append = true;
            frameNumber = 0;
        }
        
        //Update file name in database.
        server.upload(sessionid, avpkt, append);
        ++frameNumber;
    }
}
