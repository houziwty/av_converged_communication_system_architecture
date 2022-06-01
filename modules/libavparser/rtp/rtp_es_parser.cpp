#include "libavpkt.h"
#include "error_code.h"
#include "rtp-h264-unpack.h"
#include "rtp_es_parser.h"
using namespace module::av::stream;

int onpacket(void* param, const void* packet, int bytes, uint32_t timestamp, int flags)
{
//    printf("type = %x, bytes = %d, timestamp = %u.\r\n", ((const char*)packet)[0], bytes, timestamp);
    // static FILE* fd{ nullptr };
    // static char prefix[4]{ 0, 0, 0, 1 };
    // if (!fd)
    // {
    //     fopen_s(&fd, "d:\\test.264", "wb+");
    // }

    // if (fd)
    // {
    //     fwrite(prefix, 4, 1, fd);
    //     fwrite(packet, bytes, 1, fd);
    // }
    return 0;
}

RTPESParser::RTPESParser(
    ParsedDataCallback callback, const uint32_t id/* = 0*/) 
    : AVParserNode(callback, id), rtp{nullptr}
{}

RTPESParser::~RTPESParser()
{
    if (rtp)
    {
        rtp_h264_unpack_destroy(rtp);
    } 
}

int RTPESParser::input(const void* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (!rtp)
        {
            struct rtp_payload_t payload{nullptr, nullptr, &onpacket};
            rtp = rtp_h264_unpack_create(&payload, this);
        }

        if (rtp)
        {
            Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
            ret = (0 <= rtp_h264_unpack_input(rtp, (const uint8_t*)pkt->data(), pkt->bytes()) ? Error_Code_Success : Error_Code_Operate_Failure);
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }
    
    return ret;
}
