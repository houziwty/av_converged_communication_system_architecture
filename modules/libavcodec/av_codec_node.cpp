#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "decode/ffmpeg_h264_decode.h"
#include "convert/ffmpeg_picture_convert.h"
#include "av_codec.h"
#include "av_codec_node.h"
using namespace module::av::stream;

using AVCodecPtr = boost::shared_ptr<AVcodec>;
using AVCodecPtrs = UnorderedMap<const uint32_t, AVCodecPtr>;

static AVCodecPtrs codecs;

AVCodecNode::AVCodecNode()
{}

AVCodecNode::~AVCodecNode()
{}

int AVCodecNode::addConf(const AVCodecModeConf& conf)
{
	int ret{
		0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVCodecPtr codec;

		if (AVCodecType::AV_CODEC_TYPE_DECODE_H264 == conf.type)
		{
			codec = boost::make_shared<FFmpegH264Decode>(
				boost::bind(&AVCodecNode::afterCodecDataNotification, this, _1, _2), 
				conf.id);
		}
		else if (AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 == conf.type)
		{
			codec = boost::make_shared<FFmpegPictureConvert>(
				boost::bind(&AVCodecNode::afterCodecDataNotification, this, _1, _2), 
				conf.id);
		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}

		if (codec)
		{
			codecs.add(conf.id, codec);
		}
	}

	return ret;
}

int AVCodecNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVCodecPtr codec{codecs.at(id)};

		if (codec)
		{
			codecs.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int AVCodecNode::input(
	const uint32_t id/* = 0*/, 
	const AVPkt* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVCodecPtr codec{codecs.at(id)};
		ret = codec ? codec->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
