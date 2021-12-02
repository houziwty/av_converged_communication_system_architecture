#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libmedia/filter/decoder/video_decoder_filter.h"
#include "libmedia/filter/parser/av_packet_parser_filter.h"
#include "libmedia/filter/encoder/picture_encoder_filter.h"
#include "libmedia/pin.h"
#include "libmedia/filter.h"
#include "video_ps_stream_encode_image_graph.h"
using namespace module::media::av;

VideoPSStreamEncodeImageGraph::VideoPSStreamEncodeImageGraph() : Graph()
{}

VideoPSStreamEncodeImageGraph::~VideoPSStreamEncodeImageGraph()
{}

int VideoPSStreamEncodeImageGraph::createNew()
{
	int ret{Graph::createNew()};

	if (Error_Code_Success == ret)
	{
		FilterPtr avppf{boost::make_shared<AVPacketParserFilter>()};
		FilterPtr vdf{boost::make_shared<VideoDecoderFilter>()};
		FilterPtr pef{boost::make_shared<PictureEncoderFilter>()};

		if (avppf && Error_Code_Success == avppf->createNew() &&
			vdf && Error_Code_Success == vdf->createNew() && 
			pef && Error_Code_Success == pef->createNew())
		{
			filters.insert(std::make_pair(innerAVPacketParserFilterName, avppf));
			filters.insert(std::make_pair(innerVideoDecoderFilterName, vdf));
			filters.insert(std::make_pair(innerPictureEncoderFilterName, pef));
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return ret;
}

int VideoPSStreamEncodeImageGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	FilterPtr avppf{query(innerAVPacketParserFilterName)};
	FilterPtr vdf{query(innerVideoDecoderFilterName)};
	FilterPtr pef{query(innerPictureEncoderFilterName)};

	if (avppf && vdf && pef)
	{
		PinPtr avppf_out{avppf->query(innerDataOutputPinName)};
		PinPtr vdf_in{vdf->query(innerDataInputPinName)};
		PinPtr vdf_out{vdf->query(innerDataOutputPinName)};
		PinPtr pef_in{pef->query(innerDataInputPinName)};

		if (avppf_out && vdf_in && vdf_out && pef_in)
		{
			avppf_out->next(vdf_in);
			vdf_out->next(pef_in);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
