#include "error_code.h"
#include "ps_parser.h"
using namespace module::av::stream;

PSParser::PSParser(
    ParsedDataCallback callback, 
	const uint32_t id/* = 0*/) 
    : AVParser(callback, id)
{}

PSParser::~PSParser()
{}

int PSParser::input(
    const uint8_t* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
    return 0;
}
