#include "boost/checked_delete.hpp"
#include "libasio.h"
using namespace module::network::asio;
#include "libavpkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "server.h"
#include "session.h"

uint32_t Session::counter = 0;

Session::Session(Server& svr) : Libavparser(), server{svr}, sequence{0}, index{10000 + (++index)}
{
    AVParserModeConf conf{index, AVParserType::AV_PARSER_TYPE_PS_PARSER, 3 * 1024 * 1024};
    Libavparser::addConf(conf);
}

Session::~Session()
{
    Libavparser::removeConf(index);
}

int Session::addTarget(const uint32_t sid/* = 0*/)
{
    int ret{0 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        mtx.lock();
        for (int i = 0; i != sids.size(); ++i)
        {
            if (sid == sids[i])
            {
                ret = Error_Code_Object_Existed;
                break;
            }
        }

        if (Error_Code_Success == ret)
        {
            sids.push_back(sid);
        }
        mtx.unlock();
    }
    
    return ret;
}

int Session::removeTarget(const uint32_t sid/* = 0*/)
{
    int ret{0 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        mtx.lock();
        for (std::vector<uint32_t>::iterator it = sids.begin(); it != sids.end();)
        {
            if (sid == *it)
            {
                it = sids.erase(it);
                break;
            }
            else
            {
                ++it;
                ret = Error_Code_Object_Not_Exist;
            }
        }
        mtx.unlock();
    }
    
    return ret;
}

int Session::input(
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        Libavpkt avpkt;
        avpkt.input(data, bytes);
        ret = Libavparser::input(index, &avpkt);
    }
    
    return ret;
}

void Session::afterParsedDataNotification(
    const uint32_t id/* = 0*/, 
    const void* avpkt/* = nullptr*/)
{
    Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
    const uint64_t bytes{pkt->bytes()};
    const uint64_t totalBytes{bytes + 32};
    char* frameData{ new(std::nothrow) char[totalBytes] };
    *((uint32_t*)frameData) = 0xFF050301;
    *((uint32_t*)(frameData + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_VIDEO;
    *((uint32_t*)(frameData + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_ES;
    *((uint32_t*)(frameData + 12)) = bytes;
    *((uint64_t*)(frameData + 16)) = ++sequence;
    *((uint64_t*)(frameData + 24)) = 0;
    XMem().copy(pkt->data(), bytes, frameData + 32, bytes);

    for(int i = 0; i != sids.size(); ++i)
    {
        mtx.lock();
        server.sendframe(sids[i], frameData, totalBytes);
        mtx.unlock();
    }
    
    boost::checked_array_delete(frameData);
}
