#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "buffer/buffer_parser.h"
#include "ps/ps_parser.h"
#include "libavparser.h"
using namespace module::av::stream;

using AVParserNodePtr = boost::shared_ptr<AVParserNode>;
using AVParserNodePtrs = UnorderedMap<const uint32_t, AVParserNodePtr>;

static AVParserNodePtrs nodes;

Libavparser::Libavparser()
{}

Libavparser::~Libavparser()
{}

int Libavparser::addConf(const AVParserModeConf& conf)
{
	int ret{
		0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVParserNodePtr node;

		if (AVParserType::AV_PARSER_TYPE_BUFFER_PARSER == conf.type)
		{
			node = boost::make_shared<BufferParser>(
				boost::bind(&Libavparser::afterParsedDataNotification, this, _1, _2), 
				conf.id, 
				conf.cache);
		}
		else if (AVParserType::AV_PARSER_TYPE_PS_PARSER == conf.type)
		{
			node = boost::make_shared<PSParser>(
				boost::bind(&Libavparser::afterParsedDataNotification, this, _1, _2), 
				conf.id);
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

int Libavparser::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVParserNodePtr node{nodes.at(id)};

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

int Libavparser::input(
	const uint32_t id/* = 0*/, 
	const void* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVParserNodePtr node{nodes.at(id)};
		ret = node ? node->input(avpkt) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
