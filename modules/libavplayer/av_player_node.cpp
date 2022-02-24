#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "d3d/d3d.h"
#include "av_player_node.h"
using namespace module::av::stream;

using AVPlayerPtr = boost::shared_ptr<AVPlayer>;
using AVPlayerPtrs = UnorderedMap<const uint32_t, AVPlayerPtr>;

static AVPlayerPtrs avplayers;

AVPlayerNode::AVPlayerNode()
{}

AVPlayerNode::~AVPlayerNode()
{}

int AVPlayerNode::addConf(const AVPlayerModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVPlayerPtr player;

		if (AVPlayerType::AV_PLAYER_TYPE_D3D == conf.type)
		{
			player = boost::make_shared<D3D>(conf.id, conf.video.hwnd);
		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}

		if (player)
		{
			avplayers.add(conf.id, player);
		}
	}

	return ret;
}

int AVPlayerNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVPlayerPtr player{ avplayers.at(id)};

		if (player)
		{
			avplayers.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int AVPlayerNode::input(
	const uint32_t id/* = 0*/, 
	const AVPkt* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVPlayerPtr player{ avplayers.at(id)};
		ret = player ? player->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
