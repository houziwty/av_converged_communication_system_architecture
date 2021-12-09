#include "error_code.h"
#include "ps_2_es_parser.h"
using namespace module::media::av;

PS2ESParser::PS2ESParser() : AVProcessor()
{}

PS2ESParser::~PS2ESParser()
{
    destroy();
}

int PS2ESParser::createNew(void* param/* = nullptr*/)
{
    return Error_Code_Success;
}

int PS2ESParser::destroy()
{
    return Error_Code_Success;
}

int PS2ESParser::input(void* data/* = nullptr*/)
{

}
