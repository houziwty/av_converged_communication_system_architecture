#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libmedia/filter/capture/av_data_capture_filter.h"
#include "libmedia/filter/callback/av_data_callback_filter.h"
#include "libmedia/pin.h"
#include "libmedia/filter.h"
#include "graph/hksdk_video_ps_stream_encode_image_graph.h"
using namespace framework::media::av;

HKSdkVideoPSStreamEncodeImageGraph::HKSdkVideoPSStreamEncodeImageGraph(
	const int did, const std::string url, EncodeImageDataCallback callback)
	: VideoPSStreamEncodeImageGraph(), streamUrl{url}, encodeImageDataCallback{callback}, deviceId{did}
{}

HKSdkVideoPSStreamEncodeImageGraph::~HKSdkVideoPSStreamEncodeImageGraph()
{}

int HKSdkVideoPSStreamEncodeImageGraph::createNew()
{
	int ret{VideoPSStreamEncodeImageGraph::createNew()};
	int playId{Error_Code_Bad_New_Object};

	if (Error_Code_Success == ret)
	{
		FilterPtr capture{boost::make_shared<AVDataCaptureFilter>()};
		FilterPtr callback{boost::make_shared<AVDataCallbackFilter>()};

		if (capture && -1< (playId = capture->createNew(streamUrl.c_str())) &&
			callback && Error_Code_Success == callback->createNew(this))
		{
			filters.insert(std::make_pair(innerVideoCaptureFilterName, capture));
			filters.insert(std::make_pair(innerDataCallbackFilterName, callback));

			if(-1 < playId)
			{
				connectPin();
			}
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return playId;
}

int HKSdkVideoPSStreamEncodeImageGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	FilterPtr capture{query(innerVideoCaptureFilterName)};
	FilterPtr parser{query(innerAVPacketParserFilterName)};
	FilterPtr encoder{query(innerPictureEncoderFilterName)};
	FilterPtr callback{query(innerDataCallbackFilterName)};

	if (capture && parser && encoder && callback)
	{
		PinPtr cap_out{capture->query(innerDataOutputPinName)};
		PinPtr par_in{parser->query(innerDataInputPinName)};
		PinPtr enc_out{encoder->query(innerDataOutputPinName)};
		PinPtr cb_in{callback->query(innerDataInputPinName)};

		if (cap_out && par_in && enc_out && cb_in)
		{
			cap_out->next(par_in);
			enc_out->next(cb_in);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}

void HKSdkVideoPSStreamEncodeImageGraph::processDataCompleteNotification(void* data/* = nullptr*/)
{
	if(encodeImageDataCallback)
	{
		encodeImageDataCallback(deviceId, data);
	}
}
