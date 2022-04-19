#include "av_codec_node.h"
using namespace module::av::stream;

AVcodecNode::AVcodecNode(
	AVCodecDataCallback callback, const uint32_t id/* = 0*/)
	: cid{id}, avcodecDataCallback{callback}
{}

AVcodecNode::~AVcodecNode()
{}
