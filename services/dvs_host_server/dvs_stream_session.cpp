#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "dvs_stream_session.h"

DvsStreamSession::DvsStreamSession(const uint32_t id/* = 0*/)
    : sid{id}, cid{0}, did{0}
{}

DvsStreamSession::~DvsStreamSession()
{}

int DvsStreamSession::recv(
    const uint8_t* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        if (!bufferParserPtr)
        {
            BufferParserPtr bp{
                boost::make_shared<BufferParser>(
                    boost::bind(
                        &DvsStreamSession::afterParsedOneFrameNotification, this, _1, _2, _3, _4, _5, _6, _7))};
            if(bp)
            {
                bufferParserPtr.swap(bp);
            }
        }
        
        ret = (bufferParserPtr ? bufferParserPtr->append(data, bytes) : Error_Code_Bad_New_Object);
    }
    
    return ret;
}

int DvsStreamSession::send(
    const uint32_t did/* = 0*/, 
    const uint32_t cid/* = 0*/, 
    const uint8_t* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{0 < did && 0 < cid && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = (did == this->did && cid == this->cid && 0 < sid ? ASIONode::send(sid, data, bytes) : Error_Code_Operate_Failure);
    }

    return ret;
}

void DvsStreamSession::afterParsedOneFrameNotification(
    const uint32_t dataType/* = 0*/, 
    const uint32_t streamType/* = 0*/, 
    const uint32_t frameType/* = 0*/, 
    const uint32_t frameBytes/* = 0*/, 
    const uint64_t frameSeq/* = 0*/, 
    const uint64_t frameTs/* = 0*/, 
    const uint8_t* frameData/* = nullptr*/)
{
    const std::string msg{(const char*)frameData, frameBytes};
    Url streamUrl;
    int ret{streamUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        // fileLog.write(
        //     SeverityLevel::SEVERITY_LEVEL_INFO, 
        //     "Parsed stream data = [ %s ] from session = [ %d ] successfully.",  
        //     msg.c_str(), 
        //     id);
    }
    else
    {
        // fileLog.write(
        //     SeverityLevel::SEVERITY_LEVEL_ERROR, 
        //     "Parsed stream data = [ %s ] from session = [ %d ] failed, result = [ %d ].",  
        //     msg.c_str(), 
        //     id, 
        //     ret);
    }
}
