#ifdef __cplusplus
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
}
#endif//__cplusplus
#include "libavpkt.h"
#include "error_code.h"
#include "ffmpeg_h264_decode.h"
using namespace module::av::stream;

FFmpegH264Decode::FFmpegH264Decode(
    AVCodecDataCallback callback, const uint32_t id/* = 0*/) 
    : AVcodecNode(callback, id), ctx{nullptr}, codec{nullptr}, 
    iframe{nullptr}, oframe{nullptr}, obuffer{nullptr}, obytes{0}
{}

FFmpegH264Decode::~FFmpegH264Decode()
{
    uninit();
}

int FFmpegH264Decode::input(const void* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret && Error_Code_Success == init(avpkt))
    {
		Libavpkt* inpkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
		AVPacket* pkt{ av_packet_alloc() };
		if (!pkt)
		{
			return Error_Code_Bad_New_Memory;
		}
		else
		{
			pkt->data = (uint8_t*)inpkt->data();
			pkt->size = inpkt->bytes();
		}

		if (0 > avcodec_send_packet((AVCodecContext*)ctx, pkt))
		{
			return Error_Code_Operate_Failure;
		}

		const uint32_t w{ inpkt->width() }, h{ inpkt->height() };
		const uint64_t seq{ inpkt->sequence() }, ts{ inpkt->timestamp() };
		AVCodecContext* avcodecctx{ reinterpret_cast<AVCodecContext*>(ctx) };
		AVFrame* output_frame{ reinterpret_cast<AVFrame*>(oframe) };

		while (1)
		{
			int err{ avcodec_receive_frame(avcodecctx, output_frame) };

			if (AVERROR(EAGAIN) == err || AVERROR_EOF == err || 0 > err)
			{
				break;
			}

			av_image_copy_to_buffer(
				obuffer, obytes, output_frame->data, output_frame->linesize, AV_PIX_FMT_YUV420P, w, h, 1);
			Libavpkt outpkt{ 
				AVMainType::AV_MAIN_TYPE_VIDEO, 
				AVSubType::AV_SUB_TYPE_YUV420P, 
				seq, ts, w, h };
			outpkt.input(obuffer, obytes);

			if (avcodecDataCallback)
			{
				avcodecDataCallback(cid, &outpkt);
			}
		}

		av_packet_unref(pkt);
    }
    
    return ret;
}

int FFmpegH264Decode::init(const void* avpkt /* = nullptr */)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
		Libavpkt* inpkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
        const uint32_t w{ inpkt->width() }, h{ inpkt->width() };
        AVCodecContext* avcodecctx{ reinterpret_cast<AVCodecContext*>(ctx) };
		AVCodec* avcodec{ reinterpret_cast<AVCodec*>(codec) };
        
        if (0 < w && 0 < h)
        {
			if ((!avcodecctx && !avcodec) || (avcodecctx->width != w && avcodecctx->height != h))
			{
                uninit();
				avcodec = (AVCodec*)avcodec_find_decoder(AV_CODEC_ID_H264);
				avcodecctx = avcodec_alloc_context3(avcodec);

				if (avcodec && avcodecctx && !avcodec_open2(avcodecctx, avcodec, nullptr))
				{
					iframe = av_frame_alloc();
					oframe = av_frame_alloc();
					obytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, w, h, 1);
					obuffer = (uint8_t*)av_malloc(obytes);

					ctx = avcodecctx;
					codec = avcodec;
				}
			}
        }
    }

    return ctx && codec && iframe && oframe && obuffer && 0 < obytes ? Error_Code_Success : Error_Code_Operate_Failure;
}

int FFmpegH264Decode::uninit()
{
    AVCodecContext* avcodecctx{ reinterpret_cast<AVCodecContext*>(ctx) };
    if (avcodecctx)
    {
        avcodec_free_context(&avcodecctx);
        ctx = nullptr;
    }

    AVFrame* in{ reinterpret_cast<AVFrame*>(iframe) };
    if (in)
    {
        av_frame_free(&in);
        iframe = nullptr;
    }

	AVFrame* out{ reinterpret_cast<AVFrame*>(oframe) };
	if (in)
	{
		av_frame_free(&out);
		oframe = nullptr;
	}
    
    if (obuffer)
    {
        av_free(obuffer);
        obuffer = nullptr;
    }

    return Error_Code_Success;
}
