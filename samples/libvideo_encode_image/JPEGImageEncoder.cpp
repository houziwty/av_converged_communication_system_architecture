extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
}
#include "error_code.h"
#include "JPEGImageEncoder.h"

JPEGImageEncoder::JPEGImageEncoder()
	: codec{ nullptr }, ctx{ nullptr }, iframe{ nullptr }
{}

JPEGImageEncoder::~JPEGImageEncoder()
{
	avcodec_close(reinterpret_cast<AVCodecContext*>(ctx));
	av_free(reinterpret_cast<AVCodec*>(codec));
	AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
	av_frame_free(&avf);
}

int JPEGImageEncoder::input(void* data, const int w, const int h, void*& jpeg, int& bytes)
{
	int ret{ data && 0 < w && 0 < h ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (!codec && !ctx && !iframe)
		{
			codec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
			iframe = av_frame_alloc();
			AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
			avf->format = AV_PIX_FMT_YUV420P;
			avf->width = w;
			avf->height = h;

			AVCodec* avc{ reinterpret_cast<AVCodec*>(codec) };
			ctx = avcodec_alloc_context3(avc);
			AVCodecContext* avcc{ reinterpret_cast<AVCodecContext*>(ctx) };
			avcc->pix_fmt = AV_PIX_FMT_YUVJ420P;
			avcc->codec_type = AVMEDIA_TYPE_VIDEO;
			avcc->time_base.num = 1;
			avcc->time_base.den = 11;
			avcc->width = w;
			avcc->height = h;
			avcodec_open2(avcc, avc, NULL);
		}

		if (codec && ctx && iframe)
		{
			AVCodecContext* avcc{ reinterpret_cast<AVCodecContext*>(ctx) };
			AVFrame* avf{ reinterpret_cast<AVFrame*>(iframe) };
			av_image_fill_arrays(
				avf->data, avf->linesize, (const uint8_t*)data, AV_PIX_FMT_YUV420P, w, h, 1);

			if (!avcodec_send_frame(avcc, avf))
			{
				AVPacket avpkt;
				av_init_packet(&avpkt);
				if (!avcodec_receive_packet(avcc, &avpkt))
				{
					// Base64 base64;
					// e = base64.encodeBase64((const char*)avpkt.data, avpkt.size);

					// if (eSuccess == e)
					// {
					// 	data->setData(base64.getData(), base64.getDataBytes());
					// 	fp->setFrameType(framework::data::FrameType::FRAME_TYPE_JPEG);
						
					// 	if (dataNotificationCallback)
					// 	{
					// 		dataNotificationCallback(data);
					// 	}
					// }

					jpeg = avpkt.data;
					bytes = avpkt.size;
				}

				av_packet_unref(&avpkt);
			}
			else
			{
				ret = Error_Code_Operate_Failure;
			}
		}
	}

	return ret;
}
