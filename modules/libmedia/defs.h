//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : Defs
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_MEDIA_AV_DEFS_H
#define MODULE_MEDIA_AV_DEFS_H

#include <string>
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"

//针脚传输模式
//必选其一
typedef enum class tagTransportMode_t : int
{
    TRANSPORT_MODE_INPUT = 0,
    TRANSPORT_MODE_OUTPUT
}TransportMode;

//过滤器工作模式
//必选其一
typedef enum class tagWorkMode_t : int
{
    WORK_MODE_SOURCE = 0, 
    WORK_MODE_MEDIUM,
    WORK_MODE_TARGET
}WorkMode;

//已集成的过滤器名称
static const std::string innerVideoDecoderFilterName = "InnerVideoDecoderFilter";
static const std::string innerVideoRenderFilterName = "InnerVideoRenderFilter";
static const std::string innerAudioDecoderFilterName = "InnerAudioDecoderFilter";
static const std::string innerAudioPlayerFilterName = "InnerAudioPlayerFilter";
static const std::string innerImageFormatterFilterName = " innerImageFormatterFilter";
static const std::string innerPictureEncoderFilterName = " innerPictureEncoderFilter";
static const std::string innerCVAnalysisFilterName = " innerCVAnalysisFilter";
//已集成的针脚名称
static const std::string innerDataInputPinName = "innerDataInputPin";
static const std::string innerDataOutputPinName = "innerDataOutputPin";

namespace module
{
	namespace media
	{
		namespace av
		{
            class Filter;
            using FilterPtr = boost::shared_ptr<Filter>;
            using FilterPtrs = boost::unordered_map<const std::string, FilterPtr>;

            class Pin;
            using PinPtr = boost::shared_ptr<Pin>;
            using PinPtrs = boost::unordered_map<const std::string, PinPtr>;
        }
    }
}

#endif//MODULE_MEDIA_AV_DEFS_H