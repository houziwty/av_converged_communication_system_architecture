#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libmedia/filter/decoder/video_decoder_filter.h"
#include "libmedia/filter/decoder/audio_decoder_filter.h"
#include "libmedia/filter/render/video_render_filter.h"
#include "libmedia/filter/render/audio_play_filter.h"
#include "libmedia/pin.h"
#include "libmedia/filter.h"
#include "realplay_stream_graph.h"
using namespace module::media::av;

RealplayStreamGraph::RealplayStreamGraph()
	: Graph()
{}

RealplayStreamGraph::~RealplayStreamGraph()
{}

int RealplayStreamGraph::createNew()
{
	int ret{Graph::createNew()};

	if (Error_Code_Success == ret)
	{
		FilterPtr vdf{boost::make_shared<VideoDecoderFilter>()};
		FilterPtr vrf{boost::make_shared<VideoRenderFilter>()};
		FilterPtr adf{boost::make_shared<AudioDecoderFilter>()};
		FilterPtr apf{boost::make_shared<AudioPlayFilter>()};

		if (vdf && Error_Code_Success == vdf->createNew() &&
			vrf && Error_Code_Success == vrf->createNew() && 
			adf && Error_Code_Success == adf->createNew() &&
			apf && Error_Code_Success == apf->createNew())
		{
			filters.insert(std::make_pair(innerVideoDecoderFilterName, vdf));
			filters.insert(std::make_pair(innerVideoRenderFilterName, vrf));
			filters.insert(std::make_pair(innerAudioDecoderFilterName, adf));
			filters.insert(std::make_pair(innerAudioPlayerFilterName, apf));
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return ret;
}

int RealplayStreamGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	FilterPtr vdf{query(innerVideoDecoderFilterName)};
	FilterPtr vrf{query(innerVideoRenderFilterName)};
	FilterPtr adf{query(innerAudioDecoderFilterName)};
	FilterPtr apf{query(innerAudioPlayerFilterName)};

	if (vdf && vrf && adf && apf)
	{
		PinPtr vdf_out{vdf->query(innerDataOutputPinName)};
		PinPtr vrf_in{vrf->query(innerDataInputPinName)};
		PinPtr adf_out{adf->query(innerDataOutputPinName)};
		PinPtr apf_in{apf->query(innerDataInputPinName)};

		if (vdf_out && vrf_in && adf_out && apf_in)
		{
			vdf_out->next(vrf_in);
			adf_out->next(apf_in);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
