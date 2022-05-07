#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
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
    : Session(svr, sid), deviceid{did}, channelid{cid}, thread{nullptr}, frameNumber{0}
{}

UploadSession::~UploadSession()
{}

int UploadSession::run()
{
    int ret{0 < sessionid && 0 < deviceid && 0 < channelid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        thread = ThreadPool().get_mutable_instance().createNew(
            boost::bind(
                &UploadSession::sendRealplayRequestThread, this));
        ret = (thread ? Error_Code_Success : Error_Code_Bad_New_Thread);
    }
    
    return ret;
}

int UploadSession::stop()
{
    int ret{0 < sessionid && thread ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ThreadPool().get_mutable_instance().destroy(thread);
    }
    
    return ret;
}

int UploadSession::input(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        //存储数据包含32字节的头数据
        bool append{false};
        
        if (!frameNumber || frameNumber >= maxFrameNumber)
        {
            append = true;
            frameNumber = 0;
        }
        
        //Update file name in database.
        server.upload(sessionid, data, bytes, append);
        ++frameNumber;
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
    *((uint32_t*)(frameData + 4)) = 0xFF0A0000;
    *((uint32_t*)(frameData + 8)) = 0xFF0A0B00;
    *((uint32_t*)(frameData + 12)) = bytes;
    *((uint64_t*)(frameData + 16)) = ++sequence;
    *((uint64_t*)(frameData + 24)) = 0;
    XMem().copy(req.c_str(), bytes, frameData + 32, bytes);
    server.send(sessionid, frameData, totalBytes);
    boost::checked_array_delete(frameData);
}
