#include "av_parser_node.h"
using namespace module::av::stream;

AVParserNode::AVParserNode(
	ParsedDataCallback callback, const uint32_t id/* = 0*/)
	: pid{id}, parsedDataCallback{callback}
{}

AVParserNode::~AVParserNode()
{}