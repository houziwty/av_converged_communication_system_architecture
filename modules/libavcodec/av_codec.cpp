#include "av_codec.h"
using namespace module::av::stream;

AVcodec::AVcodec(
	AVCodecDataCallback callback, 
	const uint32_t id/* = 0*/)
	: cid{id}, avcodecDataCallback{callback}
{}

AVcodec::~AVcodec()
{}