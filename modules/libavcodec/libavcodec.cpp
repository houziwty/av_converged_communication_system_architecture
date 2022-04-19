#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "decode/ffmpeg_h264_decode.h"
#include "convert/ffmpeg_picture_convert.h"
#include "encode/ffmpeg_jpeg_encode.h"
#include "av_codec_node.h"
#include "libavcodec.h"
using namespace module::av::stream;

using AVCodecNodePtr = boost::shared_ptr<AVcodecNode>;
using AVCodecNodePtrs = UnorderedMap<const uint32_t, AVCodecNodePtr>;

static AVCodecNodePtrs nodes;

Libavcodec::Libavcodec()
{}

Libavcodec::~Libavcodec()
{}

int Libavcodec::addConf(const AVCodecModeConf& conf)
{
	int ret{
		0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVCodecNodePtr node;

		if (AVCodecType::AV_CODEC_TYPE_DECODE_H264 == conf.type)
		{
			node = boost::make_shared<FFmpegH264Decode>(
				boost::bind(&Libavcodec::afterCodecDataNotification, this, _1, _2), 
				conf.id);
		}
		else if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 == conf.type || 
				 AVCodecType::AV_CODEC_TYPE_YUV420P_2_RGB24 == conf.type)
		{
			node = boost::make_shared<FFmpegPictureConvert>(
				boost::bind(&Libavcodec::afterCodecDataNotification, this, _1, _2), 
				conf.id, conf.type);
		}
		else if (AVCodecType::AV_CODEC_TYPE_ENCODE_JPEG == conf.type)
		{
			node = boost::make_shared<FFmpegJPEGEncode>(
				boost::bind(&Libavcodec::afterCodecDataNotification, this, _1, _2), 
				conf.id);
		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}

		if (node)
		{
			nodes.add(conf.id, node);
		}
	}

	return ret;
}

int Libavcodec::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVCodecNodePtr node{nodes.at(id)};

		if (node)
		{
			nodes.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int Libavcodec::input(
	const uint32_t id/* = 0*/, 
	const void* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVCodecNodePtr node{nodes.at(id)};
		ret = node ? node->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
