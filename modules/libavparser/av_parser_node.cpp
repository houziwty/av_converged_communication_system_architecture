#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "buffer/buffer_parser.h"
#include "ps/ps_parser.h"
#include "av_parser_node.h"
using namespace module::av::stream;

using AVParserPtr = boost::shared_ptr<AVParser>;
using AVParserPtrs = UnorderedMap<const uint32_t, AVParserPtr>;

static AVParserPtrs parsers;

AVParserNode::AVParserNode()
{}

AVParserNode::~AVParserNode()
{}

int AVParserNode::addConf(const AVParserModeConf& conf)
{
	int ret{
		0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVParserPtr parser;

		if (AVParserType::AV_PARSER_TYPE_BUFFER_PARSER == conf.type)
		{
			parser = boost::make_shared<BufferParser>(
				boost::bind(
					&AVParserNode::afterParsedDataNotification, this, _1, _2, _3, _4, _5, _6, _7), 
				conf.id);
		}
		else if (AVParserType::AV_PARSER_TYPE_PS_PARSER == conf.type)
		{
			parser = boost::make_shared<PSParser>(
				boost::bind(
					&AVParserNode::afterParsedDataNotification, this, _1, _2, _3, _4, _5, _6, _7), 
				conf.id);
		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}

		if (parser)
		{
			parsers.add(conf.id, parser);
		}
	}

	return ret;
}

int AVParserNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVParserPtr parser{parsers.at(id)};

		if (parser)
		{
			parsers.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int AVParserNode::input(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVParserPtr parser{parsers.at(id)};
		ret = parser ? parser->input(data, bytes) : Error_Code_Object_Not_Exist;
	}

	return ret;
}
