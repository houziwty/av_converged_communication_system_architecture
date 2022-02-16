#include "av_parser.h"
using namespace module::av::stream;

AVParser::AVParser(
	ParsedDataCallback callback, 
	const uint32_t id/* = 0*/)
	: pid{id}, parsedDataCallback{callback}
{}

AVParser::~AVParser()
{}