#ifdef __cplusplus
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
}
#endif//__cplusplus
#include "av_pkt.h"
#include "error_code.h"
#include "ffmpeg_jpeg_encode.h"
using namespace module::av::stream;

FFmpegJPEGEncode::FFmpegJPEGEncode(
    AVCodecDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVcodec(callback, id), ctx{nullptr}, codec{nullptr}, iframe{nullptr}
{}

FFmpegJPEGEncode::~FFmpegJPEGEncode()
{
    uninit();
}

int FFmpegJPEGEncode::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret && Error_Code_Success == init(avpkt))
    {
		const uint32_t w{ avpkt->width() }, h{ avpkt->height() };
		const uint64_t seq{ avpkt->sequence() }, ts{ avpkt->timestamp() };

		AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
		av_image_fill_arrays(
			avf->data, avf->linesize, (const uint8_t*)avpkt->data(), AV_PIX_FMT_YUV420P, w, h, 1);

		if (0 > avcodec_send_frame((AVCodecContext*)ctx, avf))
		{
			return Error_Code_Operate_Failure;
		}

		while (1)
		{
			AVPacket* out{av_packet_alloc()};
			if (!out)
			{
				break;
			}
			
			int err{ avcodec_receive_packet((AVCodecContext*)ctx, out) };

			if (AVERROR(EAGAIN) == err || AVERROR_EOF == err || 0 > err)
			{
				av_packet_free(&out);
				break;
			}

			AVPkt _avpkt_{ 
				AVMainType::AV_MAIN_TYPE_VIDEO, 
				AVSubType::AV_SUB_TYPE_JPEG, 
				seq, ts, w, h };
			_avpkt_.input(out->data, out->size);

			if (avcodecDataCallback)
			{
				avcodecDataCallback(cid, &_avpkt_);
			}

			av_packet_free(&out);
		}
    }
    
    return ret;
}

int FFmpegJPEGEncode::init(const AVPkt* avpkt /* = nullptr */)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const uint32_t w{ avpkt->width() }, h{ avpkt->height() };
        AVCodecContext* avcodecctx{ reinterpret_cast<AVCodecContext*>(ctx) };
		AVCodec* avcodec{ reinterpret_cast<AVCodec*>(codec) };
        
        if (0 < w && 0 < h)
        {
			if ((!avcodecctx && !avcodec) || (avcodecctx->width != w && avcodecctx->height != h))
			{
                uninit();
				avcodec = (AVCodec*)avcodec_find_encoder(AV_CODEC_ID_MJPEG);
				avcodecctx = avcodec_alloc_context3(avcodec);
				avcodecctx->pix_fmt = AV_PIX_FMT_YUVJ420P;
				avcodecctx->codec_type = AVMEDIA_TYPE_VIDEO;
				avcodecctx->time_base.num = 1;
				avcodecctx->time_base.den = 11;
				avcodecctx->width = w;
				avcodecctx->height = h;
				iframe = av_frame_alloc();

				if (avcodec && avcodecctx && iframe && !avcodec_open2(avcodecctx, avcodec, nullptr))
				{
					AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
					avf->format = AV_PIX_FMT_YUV420P;
					avf->width = w;
					avf->height = h;

					ctx = avcodecctx;
					codec = avcodec;
				}
			}
        }
    }

    return ctx && codec ? Error_Code_Success : Error_Code_Operate_Failure;
}

int FFmpegJPEGEncode::uninit()
{
    AVCodecContext* avcodecctx{ reinterpret_cast<AVCodecContext*>(ctx) };
    if (avcodecctx)
    {
        avcodec_free_context(&avcodecctx);
        ctx = nullptr;
    }

	AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
	if (avf)
	{
		av_frame_free(&avf);
		avf = nullptr;
	}

    return Error_Code_Success;
}
