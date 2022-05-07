#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#ifdef _WINDOWS
#include "graph/av_realplay_graph.h"
#include "graph/av_playback_graph.h"
#endif//_WINDOWS
#include "graph/av_grab_rgb_image_graph.h"
#include "graph/av_grab_ps_graph.h"
#include "graph/av_grab_jpeg_image_graph.h"
#include "filter/av_filter.h"
#include "libav.h"
using namespace module::av::stream;

using AVGraphPtr = boost::shared_ptr<AVGraph>;
using AVGraphPtrs = UnorderedMap<const uint32_t, AVGraphPtr>;

static AVGraphPtrs graphs;

Libav::Libav()
{}

Libav::~Libav()
{}

int Libav::addConf(const AVModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph;

		if (AVModeType::AV_MODE_TYPE_GRAB_BRG24 == conf.type || 
			AVModeType::AV_MODE_TYPE_GRAB_RGB24 == conf.type)
		{
			graph = boost::make_shared<AVGrabRGBImageGraph>();
		}
		else if (AVModeType::AV_MODE_TYPE_GRAB_PS == conf.type)
		{
			graph = boost::make_shared<AVGrabPSGraph>();
		}
		else if (AVModeType::AV_MODE_TYPE_GRAB_JPEG == conf.type)
		{
			graph = boost::make_shared<AVGrabJPEGImageGraph>();
		}
#ifdef _WINDOWS
		else if (AVModeType::AV_MODE_TYPE_REALPLAY == conf.type)
		{
			graph = boost::make_shared<AVRealplayGraph>();
		}
		else if (AVModeType::AV_MODE_TYPE_PLAYBACK == conf.type)
		{
			graph = boost::make_shared<AVPlaybackGraph>();
		}
#endif//_WINDOWS
		else if (AVModeType::AV_MODE_TYPE_DOWNLOAD == conf.type)
		{
//			graph = boost::make_shared<AVRealplayGraph>();
		}

		if (graph)
		{
			ret = graph->createNew(conf);
			if (Error_Code_Success == ret)
			{
				graphs.add(conf.id, graph);
			}
		}
	}

	return ret;
}

int Libav::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph{graphs.at(id)};

		if (graph)
		{
			ret = graph->destroy(id);
			graphs.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int Libav::input(
	const uint32_t id/* = 0*/, 
	const void* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVGraphPtr graph{graphs.at(id)};

		if (graph)
		{
			AVFilterRef source{graph->query(av_buffer_parser_filter_name)};
			ret = (!source.expired() ? source.lock()->input(id, avpkt) : Error_Code_Object_Not_Exist);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int Libav::control(
	const uint32_t id/* = 0*/,
	const PlayControlType type/* = PlayControlType::PLAY_CONTROL_TYPE_NONE*/,
	const int32_t param/* = 0*/)
{
	int ret{ 0 < id && PlayControlType::PLAY_CONTROL_TYPE_NONE < type ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph{ graphs.at(id) };

		if (graph)
		{
			AVFilterRef source{ graph->query(av_frame_play_controller_filter_name) };
			ret = (!source.expired() ? source.lock()->control(id, type, param) : Error_Code_Object_Not_Exist);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
