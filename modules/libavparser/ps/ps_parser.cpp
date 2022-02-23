#include "av/mpeg/mpeg-ps.h"
#include "av/mpeg/mpeg-ts-proto.h"
#include "av_pkt.h"
#include "error_code.h"
#include "ps_parser.h"
using namespace module::av::stream;

PSParser::PSParser(
    ParsedDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVParser(callback, id), ps_demuxer{nullptr}
{}

PSParser::~PSParser()
{
    if (ps_demuxer)
    {
        ps_demuxer_destroy(reinterpret_cast<ps_demuxer_t*>(ps_demuxer));
    } 
}

int PSParser::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (!ps_demuxer)
        {
            struct ps_demuxer_notify_t notify{};
            ps_demuxer = ps_demuxer_create(&PSParser::parsedPSPacketCallback, this);
        }

        if (ps_demuxer)
        {
            ret = 
                (0 <= ps_demuxer_input(
                    reinterpret_cast<ps_demuxer_t*>(ps_demuxer), 
                    (const uint8_t*)avpkt->data(), 
                    avpkt->bytes()) ? Error_Code_Success : Error_Code_Operate_Failure);
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }
    
    return ret;
}

int PSParser::parsedPSPacketCallback(
    void* param, 
    int stream, 
    int codecid, 
    int flags, 
    int64_t pts, 
    int64_t dts, 
    const void* data, 
    size_t bytes)
{
    PSParser* parser{reinterpret_cast<PSParser*>(param)};

    if (parser && 0xe0 == stream && 0x1b == codecid && 0 == flags)
    {
        if (parser->parsedDataCallback)
        {
            AVMainType maintype{AVMainType::AV_MAIN_TYPE_NONE};

            if(PSI_STREAM_H264 == codecid || PSI_STREAM_H265 == codecid)
            {
                maintype = AVMainType::AV_MAIN_TYPE_VIDEO;
            }

            AVSubType subtype{AVSubType::AV_SUB_TYPE_NONE};

            if (1 == flags)
            {
                subtype = AVSubType::AV_SUB_TYPE_IDR;
            }

            AVPkt avpkt{maintype, subtype, pts, dts};
            if(Error_Code_Success == avpkt.input(data, bytes) && 
                parser->parsedDataCallback)
            {
                parser->parsedDataCallback(parser->pid, &avpkt);
            }
        }
    }
    
    return Error_Code_Success;
}
