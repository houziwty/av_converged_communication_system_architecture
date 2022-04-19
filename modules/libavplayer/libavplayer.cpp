#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "d3d/d3d.h"
#include "av_player_node.h"
#include "libavplayer.h"
using namespace module::av::stream;

using AVPlayerNodePtr = boost::shared_ptr<AVPlayerNode>;
using AVPlayerNodePtrs = UnorderedMap<const uint32_t, AVPlayerNodePtr>;

static AVPlayerNodePtrs nodes;

Libavplayer::Libavplayer()
{}

Libavplayer::~Libavplayer()
{}

int Libavplayer::addConf(const AVPlayerModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVPlayerNodePtr node;

		if (AVPlayerType::AV_PLAYER_TYPE_D3D == conf.type)
		{
			node = boost::make_shared<D3D>(
				conf.id, conf.video.hwnd, reinterpret_cast<ExtendDrawInfo*>(conf.video.areas));
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

int Libavplayer::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVPlayerNodePtr node{ nodes.at(id)};

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

int Libavplayer::input(
	const uint32_t id/* = 0*/, 
	const void* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVPlayerNodePtr node{ nodes.at(id)};
		ret = node ? node->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
