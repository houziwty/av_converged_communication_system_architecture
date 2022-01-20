#include "error_code.h"
#include "avp.h"
using namespace module::media::av;

AVProcessor::AVProcessor()
{}

AVProcessor::~AVProcessor()
{}

int AVProcessor::input(FramePtr frame) const
{
	return Error_Code_Success;
}
