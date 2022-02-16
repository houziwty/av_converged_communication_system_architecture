#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "dvs_stream_session.h"

DvsStreamSession::DvsStreamSession(FileLog& log, const uint32_t id/* = 0*/)
    : sid{id}, cid{0}, did{0}, fileLog{log}
{}

DvsStreamSession::~DvsStreamSession()
{}

int DvsStreamSession::recv(
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
    }
    
    return ret;
}

void DvsStreamSession::getIDs(uint32_t& sid, uint32_t& did, uint32_t& cid)
{
    sid = this->sid;
    did = this->did;
    cid = this->cid;
}

void DvsStreamSession::afterParsedOneFrameNotification(
    const uint32_t dataType/* = 0*/, 
    const uint32_t streamType/* = 0*/, 
    const uint32_t frameType/* = 0*/, 
    const uint32_t frameBytes/* = 0*/, 
    const uint64_t frameSeq/* = 0*/, 
    const uint64_t frameTs/* = 0*/, 
    const void* frameData/* = nullptr*/)
{
    const std::string msg{(const char*)frameData, frameBytes};
    Url url;
    int ret{url.parse(msg)};

    if(Error_Code_Success == ret)
    {
        //realplay://1?command=1&channel=1&stream=0

        int command{-1};
        did = atoi(url.getHost().c_str());
        const std::vector<ParamItem> params{url.getParameters()};

        for(int i  = 0; i != params.size(); ++i)
        {
            if(!params[i].key.compare("command"))
            {
                command = atoi(params[i].value.c_str());
            }
            else if(!params[i].key.compare("channel"))
            {
                cid = atoi(params[i].value.c_str());
            }
        }

        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Parsed stream play command = [ %d ], device ID = [ %d ], channel ID = [ %d ] from session = [ %d ] successfully.",  
            command, 
            did, 
            cid, 
            sid);
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed stream play url = [ %s ] from session = [ %d ] failed, result = [ %d ].",  
            (const char*)frameData, 
            sid, 
            ret);
    }
}
