#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/json.hpp"
#include "libavpkt.h"
using namespace module::av::stream;
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"
#include "download_session.h"

DownloadSession::DownloadSession(
    Server& svr, 
    const uint32_t sid/* = 0*/) 
    : Session(svr, sid), Libavparser()
{}

DownloadSession::~DownloadSession()
{}

int DownloadSession::run()
{
    int ret{0 < sessionid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        AVParserModeConf conf{sessionid, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER};
        ret = Libavparser::addConf(conf);
    }
    
    return ret;
}

int DownloadSession::stop()
{
    int ret{0 < sessionid ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = Libavparser::removeConf(sessionid);
    }
    
    return ret;
}

int DownloadSession::input(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        Libavpkt pkt;
        ret = pkt.input(data, bytes);

        if (Error_Code_Success == ret)
        {
            ret = Libavparser::input(sessionid, &pkt);
        }
    }
    
    return ret;
}

void DownloadSession::afterParsedDataNotification(
    const uint32_t id/* = 0*/, 
    const void* avpkt/* = nullptr*/)
{
    Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
    Url url;
    int ret{url.parse(pkt->data(), pkt->bytes())};

    if(Error_Code_Success == ret)
    {
        //download://client_id?data={"command":"mec.record.download","name":"ABCD1234","timestamp":"123456"}
        const std::vector<Parameter> params{url.parameters()};

        for(int i  = 0; i != params.size(); ++i)
        {
            if(!params[i].key.compare("data"))
            {
                try
                {
                    auto o{ boost::json::parse(params[i].value).as_object() };
                    auto name{ o.at("name").as_string() };
                }
                catch (...)
                {
                    // log.write(
                    //     SeverityLevel::SEVERITY_LEVEL_WARNING,
                    //     "Bad play download stream request url [ %s ].", (const char*)(pkt->data()));
                }

                break;
            }
        }
    }

    //Terminal
    if(Error_Code_Success == ret)
    {
        // log.write(
        //     SeverityLevel::SEVERITY_LEVEL_INFO,
        //     "Play download stream [ %d_%d ] from session [ %d ] successfully.", did, cid, id);
    }
    else
    {
        // log.write(
        //     SeverityLevel::SEVERITY_LEVEL_WARNING,
        //     "Play download stream [ %d_%d ] from session [ %d ] failed, result = [ %d ].", did, cid, id);
    }
}
