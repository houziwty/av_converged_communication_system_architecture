#include "libavp/av_frame.h"
#include "libavp/ps_2_es_parser.h"
#include "error_code.h"
#include "av_packet_parser_filter.h"
using namespace module::media::av;

AVPacketParserFilter::AVPacketParserFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

AVPacketParserFilter::~AVPacketParserFilter()
{}

int AVPacketParserFilter::input(void* data/* = nullptr*/)
{
	int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret && !avprocessor)
	{
		AVFrame* avframe{reinterpret_cast<AVFrame*>(data)};
		const MainType mt{avframe->getMainType()};

		if(MainType::MAIN_TYPE_HIKVISION_PS == mt)
		{
			AVProcessorPtr ptr{boost::make_shared<PS2ESParser>()};
			if(ptr)
			{
				avprocessor.swap(ptr);
			}
		}
		else
		{
			ret = Error_Code_Not_Support;
		}
	}

	return Error_Code_Success == ret ? Filter::input(data) : ret;
}
