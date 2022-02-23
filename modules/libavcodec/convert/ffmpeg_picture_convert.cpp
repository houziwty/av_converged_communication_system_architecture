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
    : AVcodec(callback, id), in{nullptr}, out{nullptr}, rgb24{nullptr}, rgb24Bytes{0}, cvt{nullptr}
{}

FFmpegPictureConvert::~FFmpegPictureConvert()
{
    uninitConvert();
}

int FFmpegPictureConvert::input(const AVPkt* avpkt/* = nullptr*/)
{
    int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (!cvt)
        {
            ret = initConvert();
        }

        if (cvt)
        {
            av_image_fill_arrays(
                ((AVFrame*)in)->data, ((AVFrame*)in)->linesize, (const uint8_t* const)avpkt->data(), AV_PIX_FMT_YUV420P, 1920, 1080, 1);
            int height{
                sws_scale(
                    (SwsContext*)cvt, (uint8_t const* const*)(((AVFrame*)in)->data), ((AVFrame*)in)->linesize, 0, 
                    1080, ((AVFrame*)out)->data, ((AVFrame*)out)->linesize) };

            if (0 < height)
            {
				AVPkt _avpkt{ AVMainType::AV_MAIN_TYPE_IMAGE, AVSubType::AV_SUB_TYPE_BGR24 };
                _avpkt.input(rgb24, rgb24Bytes);

				if (avcodecDataCallback)
				{
					avcodecDataCallback(cid, &_avpkt);
				}
            }
        }
    }
    
    return ret;
}

int FFmpegPictureConvert::initConvert()
{
    in = av_frame_alloc();
    out = av_frame_alloc();
    cvt = sws_getContext(
        1920, 1080,/*codecctx->width, codecctx->height*/ AV_PIX_FMT_YUV420P, 
        1920, 1080,/*codecctx->width, codecctx->height*/ AV_PIX_FMT_RGB24, 
        SWS_BICUBIC, NULL, NULL, NULL);
    rgb24Bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 1920, 1080,/*codecctx->width, codecctx->height*/ 1);
    rgb24 = (uint8_t*)av_malloc(rgb24Bytes);
    av_image_fill_arrays(
        ((AVFrame*)out)->data, ((AVFrame*)out)->linesize, rgb24, AV_PIX_FMT_RGB24,
        1920, 1080,/*codecctx->width, codecctx->height*/ 1);

    return in && out && cvt && rgb24 && 0 < rgb24Bytes ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int FFmpegPictureConvert::uninitConvert()
{
    return Error_Code_Success;
}
