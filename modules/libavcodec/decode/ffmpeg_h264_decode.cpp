#ifdef __cplusplus
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
#endif//__cplusplus
#include "av_pkt.h"
#include "error_code.h"
#include "ffmpeg_h264_decode.h"
using namespace module::av::stream;

FFmpegH264Decode::FFmpegH264Decode(
    AVCodecDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVcodec(callback, id), ctx{nullptr}, codec{nullptr}, bgr24{nullptr}, cvt{nullptr}, bgr24Frame{nullptr}, picture{nullptr}
{}

FFmpegH264Decode::~FFmpegH264Decode()
{
    uninitDecode();
}

int FFmpegH264Decode::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (!ctx && !codec && !cvt)
        {
            ret = initDecode();
        }

        if (ctx && codec/* && cvt*/)
        {
            AVPacket* pkt{av_packet_alloc()};
			pkt->data = (uint8_t*)avpkt->data();
			pkt->size = avpkt->bytes();

			if (0 > avcodec_send_packet((AVCodecContext*)ctx, pkt))
			{
				return Error_Code_Operate_Failure;
			}

			while(1)
            {
                int err{avcodec_receive_frame((AVCodecContext*)ctx, (AVFrame*)picture)};

				if (AVERROR(EAGAIN) == err || AVERROR_EOF == err || 0 > err)
				{
					break;
				}

                static FILE* fd{ nullptr };
                if (!fd)
                {
                    fd = fopen("d:\\decode.yuv", "wb+");
                }
                fwrite(((AVFrame*)picture)->data, ((AVFrame*)picture)->linesize, 1, fd);
            }

            av_packet_unref(pkt);
        }
    }
    
    return ret;
}

int FFmpegH264Decode::initDecode()
{
    int ret{Error_Code_Success};
//    av_register_all();
    const AVCodec* avcodec{avcodec_find_decoder(AV_CODEC_ID_H264)};
    AVCodecContext* avcodecctx{avcodec_alloc_context3(avcodec)};

    if (avcodec && avcodecctx)
    {
        ret = (!avcodec_open2(avcodecctx, avcodec, nullptr) ? Error_Code_Success : Error_Code_Init_Sdk_Failure);

        if (Error_Code_Success == ret)
        {
            bgr24Frame = av_frame_alloc();
            picture = av_frame_alloc();
//             bgr24 = (uint8_t*)av_malloc(
//                 av_samples_get_buffer_size(AVPixelFormat::AV_PIX_FMT_BGR24, 1920, 1080,/*codecctx->width, codecctx->height*/ 1));
//             av_samples_fill_arrays(
//                 ((AVFrame*)bgr24Frame)->data, ((AVFrame*)bgr24Frame)->linesize, bgr24, AV_PIX_FMT_BGR24, 
//                 1920, 1080,/*codecctx->width, codecctx->height*/ 1);
// //	        struct SwsContext *img_convert_ctx;
// 	        cvt = sws_getContext(
//                 1920, 1080,/*codecctx->width, codecctx->height*/ AV_PIX_FMT_NV12, 
//                 1920, 1080,/*codecctx->width, codecctx->height*/ AV_PIX_FMT_BGR24, 
//                 SWS_BICUBIC, NULL, NULL, NULL);

            ctx = (void*)avcodecctx;
            codec = (void*)avcodec;
        }
    }
    else
    {
        ret = Error_Code_Init_Sdk_Failure;
    }

    return ret;
}

int FFmpegH264Decode::uninitDecode()
{
    return Error_Code_Success;
}
