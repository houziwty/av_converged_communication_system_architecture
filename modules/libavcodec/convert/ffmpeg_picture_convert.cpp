#ifdef __cplusplus
extern "C"
{
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#endif//__cplusplus
#include "libavpkt.h"
#include "error_code.h"
#include "ffmpeg_picture_convert.h"
using namespace module::av::stream;

FFmpegPictureConvert::FFmpegPictureConvert(
	AVCodecDataCallback callback, 
	const uint32_t id /* = 0 */, 
	const AVCodecType type /* = AVCodecType::AV_CODEC_TYPE_NONE */)
    : AVcodecNode(callback, id), cvt{ nullptr }, iframe{ nullptr }, 
    oframe{ nullptr }, obuffer{ nullptr }, obytes{ 0 }, width{0}, height{0}, codecType{type}
{}

FFmpegPictureConvert::~FFmpegPictureConvert()
{
    uninit();
}

int FFmpegPictureConvert::input(const void* avpkt/* = nullptr*/)
{
    int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

    if(Error_Code_Success == ret && Error_Code_Success == init(avpkt))
    {
		Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
		const uint32_t w{ pkt->width() }, h{ pkt->height() };
		const uint64_t seq{ pkt->sequence() }, ts{ pkt->timestamp() };
		SwsContext* sws{ reinterpret_cast<SwsContext*>(cvt) };
		AVFrame* input_frame{ reinterpret_cast<AVFrame*>(iframe) };
		AVFrame* output_frame{ reinterpret_cast<AVFrame*>(oframe) };

		AVPixelFormat in{ AVPixelFormat::AV_PIX_FMT_NONE };
		if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 == codecType || 
			AVCodecType::AV_CODEC_TYPE_YUV420P_2_RGB24 == codecType)
		{
			in = AVPixelFormat::AV_PIX_FMT_YUV420P;
		}

		av_image_fill_arrays(
			input_frame->data, input_frame->linesize, (const uint8_t* const)pkt->data(), in, w, h, 1);

		int height{
			sws_scale(
				sws, (uint8_t const* const*)(input_frame->data), input_frame->linesize, 0, h, output_frame->data, output_frame->linesize) };
		if (0 < height)
		{
			AVSubType subtype{ AVSubType::AV_SUB_TYPE_NONE };
			if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 == codecType)
			{
				subtype = AVSubType::AV_SUB_TYPE_BGR24;
			}
			else if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_RGB24 == codecType)
			{
				subtype = AVSubType::AV_SUB_TYPE_RGB24;
			}

			Libavpkt outpkt{ 
				AVMainType::AV_MAIN_TYPE_IMAGE, subtype, seq, ts, w, h };
			outpkt.input(obuffer, obytes);

			if (avcodecDataCallback)
			{
				avcodecDataCallback(cid, &outpkt);
			}
		}
    }
    
    return ret;
}

int FFmpegPictureConvert::init(const void* avpkt /* = nullptr */)
{
    int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

    if (Error_Code_Success == ret)
    {
		Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
        const uint32_t w{ pkt->width() }, h{ pkt->height() };
        SwsContext* sws{ reinterpret_cast<SwsContext*>(cvt) };
		AVPixelFormat in{ AVPixelFormat::AV_PIX_FMT_NONE }, out{ AVPixelFormat::AV_PIX_FMT_NONE };
		if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 == codecType)
		{
			in = AVPixelFormat::AV_PIX_FMT_YUV420P;
			out = AVPixelFormat::AV_PIX_FMT_BGR24;
		}
		else if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_RGB24 == codecType)
		{
			in = AVPixelFormat::AV_PIX_FMT_YUV420P;
			out = AVPixelFormat::AV_PIX_FMT_RGB24;
		}

		if (AVPixelFormat::AV_PIX_FMT_NONE == in || AVPixelFormat::AV_PIX_FMT_NONE == out)
		{
			return Error_Code_Method_Not_Support;
		}

        if (0 < w && 0 < h)
        {
            if (!sws || (width != w && height != h))
            {
                uninit();
				sws = sws_getContext(
                    w, h, in, w, h, out, SWS_BICUBIC, NULL, NULL, NULL);

                if (sws)
                {
					iframe = av_frame_alloc();
					oframe = av_frame_alloc();
					obytes = av_image_get_buffer_size(out, w, h, 1);
					obuffer = (uint8_t*)av_malloc(obytes);
					av_image_fill_arrays(
						((AVFrame*)oframe)->data, ((AVFrame*)oframe)->linesize, obuffer, out, w, h, 1);

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
