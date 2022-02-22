#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "d3d/d3d.h"
#include "video_render_node.h"
using namespace module::av::stream;

using VideoRenderPtr = boost::shared_ptr<VideoRender>;
using VideoRenderPtrs = UnorderedMap<const uint32_t, VideoRenderPtr>;

static VideoRenderPtrs videorenders;

VideoRenderNode::VideoRenderNode()
{}

VideoRenderNode::~VideoRenderNode()
{}

int VideoRenderNode::addConf(const VideoRenderModeConf& conf)
{
	int ret{
		0 < conf.id && conf.hwnd ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		VideoRenderPtr render{
			boost::make_shared<D3D>(conf.hwnd, conf.id) };

		if (render)
		{
			videorenders.add(conf.id, render);
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}

	return ret;
}

int VideoRenderNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		VideoRenderPtr render{ videorenders.at(id)};

		if (render)
		{
			videorenders.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int VideoRenderNode::input(
	const uint32_t id/* = 0*/, 
	const AVPkt* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		VideoRenderPtr render{ videorenders.at(id) };
		ret = render ? render->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
