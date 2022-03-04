#ifdef __cplusplus
extern "C"
{
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#endif//__cplusplus
#include "av_pkt.h"
#include "error_code.h"
#include "ffmpeg_picture_convert.h"
using namespace module::av::stream;

FFmpegPictureConvert::FFmpegPictureConvert(
    AVCodecDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVcodec(callback, id), cvt{ nullptr }, iframe{ nullptr }, 
    oframe{ nullptr }, obuffer{ nullptr }, obytes{ 0 }, width{0}, height{0}
{}

FFmpegPictureConvert::~FFmpegPictureConvert()
{
    uninit();
}

int FFmpegPictureConvert::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

    if(Error_Code_Success == ret && Error_Code_Success == init(avpkt))
    {
		const uint32_t w{ avpkt->width() }, h{ avpkt->height() };
		const uint64_t seq{ avpkt->sequence() }, ts{ avpkt->timestamp() };
		SwsContext* sws{ reinterpret_cast<SwsContext*>(cvt) };
		AVFrame* input_frame{ reinterpret_cast<AVFrame*>(iframe) };
		AVFrame* output_frame{ reinterpret_cast<AVFrame*>(oframe) };
		av_image_fill_arrays(
			input_frame->data, input_frame->linesize, (const uint8_t* const)avpkt->data(), AV_PIX_FMT_YUV420P, w, h, 1);

		int e = sws_scale_frame(sws, output_frame, input_frame);
		if (!e)
		{
			av_image_copy_to_buffer(
				obuffer, obytes, output_frame->data, output_frame->linesize, AV_PIX_FMT_YUV420P, w, h, 1);
			AVPkt _avpkt_{ 
				AVMainType::AV_MAIN_TYPE_IMAGE, 
				AVSubType::AV_SUB_TYPE_BGR24, 
				seq, ts, w, h };
			_avpkt_.input(obuffer, obytes);

			if (avcodecDataCallback)
			{
				avcodecDataCallback(cid, &_avpkt_);
			}
		}
    }
    
    return ret;
}

int FFmpegPictureConvert::init(const AVPkt* avpkt /* = nullptr */)
{
    int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

    if (Error_Code_Success == ret)
    {
        const uint32_t w{ avpkt->width() }, h{ avpkt->height() };
        SwsContext* sws{ reinterpret_cast<SwsContext*>(cvt) };

        if (0 < w && 0 < h)
        {
            if (!sws || (width != w && height != h))
            {
                uninit();
				sws = sws_getContext(
                    w, h, AV_PIX_FMT_YUV420P,w, h, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

                if (sws)
                {
					iframe = av_frame_alloc();
					oframe = av_frame_alloc();
					obytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, w, h, 1);
					obuffer = (uint8_t*)av_malloc(obytes);

                    cvt = sws;
                    width = w;
                    height = h;
                }
            }
        }
    }

    return cvt && iframe && oframe && 0 < obytes ? Error_Code_Success : Error_Code_Operate_Failure;
}

int FFmpegPictureConvert::uninit()
{
    SwsContext* sws{ reinterpret_cast<SwsContext*>(cvt) };
    if (sws)
    {
        sws_freeContext(sws);
        sws = nullptr;
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
