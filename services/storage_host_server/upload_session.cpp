#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "libasio.h"
using namespace module::network::asio;
#include "libavpkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "server.h"
#include "upload_session.h"

UploadSession::UploadSession(Server& svr, const uint32_t id/* = 0*/) 
    : server{svr}, sid{id}, sequence{0}, realplayThread{nullptr}
{}

UploadSession::~UploadSession()
{}

int UploadSession::run(
    const uint32_t did/* = 0*/, 
    const uint32_t cid/* = 0*/)
{
    int ret{0 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        realplayThread = ThreadPool().get_mutable_instance().createNew(
            boost::bind(
                &UploadSession::sendRealplayRequestThread, this, did, cid));
    }
    
    return ret;
}

int UploadSession::stop()
{
    int ret{0 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ThreadPool().get_mutable_instance().destroy(realplayThread);
    }
    
    return ret;
}

int UploadSession::input(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        /* code */
    }
    
    return ret;
}

void UploadSession::sendRealplayRequestThread(
    const uint32_t did/* = 0*/, 
    const uint32_t cid/* = 0*/)
{
    int ret{0 < did && 0 < cid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string req{
            (boost::format("realplay://%d?command=1&channel=%d&stream=0") % did % cid).str()};
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
        ret = server.send(sid, frameData, totalBytes);
        boost::checked_array_delete(frameData);
    }
}
