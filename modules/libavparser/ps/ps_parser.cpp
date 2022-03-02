#include "av/mpeg/mpeg-ps.h"
#include "av/mpeg/mpeg-ts-proto.h"
#include "av/sps/sps_parser.h"
#include "av_pkt.h"
#include "error_code.h"
#include "ps_parser.h"
using namespace module::av::stream;

PSParser::PSParser(
    ParsedDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVParser(callback, id), demuxer{nullptr}, sequence{0}
{}

PSParser::~PSParser()
{
    if (demuxer)
    {
        ps_demuxer_destroy(reinterpret_cast<ps_demuxer_t*>(demuxer));
    } 
}

int PSParser::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (!demuxer)
        {
            struct ps_demuxer_notify_t notify{};
            demuxer = ps_demuxer_create(&PSParser::parsedPSPacketCallback, this);
        }

        if (demuxer)
        {
            ret = 
                (0 <= ps_demuxer_input(
                    reinterpret_cast<ps_demuxer_t*>(demuxer), 
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
            int w{0}, h{0}, ret{0};

            if(PSI_STREAM_H264 == codecid)
            {
                sps_info_struct sps{ 0 };
                if (1 == h264_parse_sps((uint8_t*)data + 4, bytes, &sps))
                {
                    w = sps.width;
                    h = sps.height;
                }
                
                maintype = AVMainType::AV_MAIN_TYPE_VIDEO;
            }
            else if(PSI_STREAM_H265 == codecid)
            {
                maintype = AVMainType::AV_MAIN_TYPE_VIDEO;
            }

            AVPkt avpkt{maintype, AVSubType::AV_SUB_TYPE_NONE, ++parser->sequence, static_cast<uint64_t>(dts)};
            if(Error_Code_Success == avpkt.input(data, bytes) && 
                parser->parsedDataCallback)
            {
                parser->parsedDataCallback(parser->pid, &avpkt);
            }
        }
    }
    
    return Error_Code_Success;
}
