//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : AVStream库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_AV_STREAM_GRAPH_DEFS_H
#define MODULE_AV_STREAM_GRAPH_DEFS_H

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV_STREAM
#define AV_STREAM_EXPORT __declspec(dllimport)
#else
#define AV_STREAM_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_STREAM
#elif defined(__linux__)
#define AV_STREAM_EXPORT
#endif//_WINDOWS

#include <string>
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"
#include "boost/weak_ptr.hpp"

namespace module
{
	namespace avstream
	{
        class AVGraph;
        class AVFilter;
        class AVPin;

        using AVFilterPtr = boost::shared_ptr<AVFilter>;
        using AVFilterRef = boost::weak_ptr<AVFilter>;
        using AVFilterPtrs = boost::unordered_map<const std::string, AVFilterPtr>;
        using AVPinPtr = boost::shared_ptr<AVPin>;
        using AVPinRef = boost::weak_ptr<AVPin>;
        using AVPinPtrs = boost::unordered_map<const std::string, AVPinPtr>;

        //数据回调
        typedef boost::function<void(void*)> AVStreamResultCallback;

        //过滤器名称
        static const std::string AVSourceFilterName = "AVSourceFilter";
        static const std::string AVSplitterFilterName = "AVSplitterFilter";
        static const std::string VideoDecoderFilterName = "VideoDecoderFilter";
        static const std::string VideoRenderFilterName = "VideoRenderFilter";
        static const std::string AudioDecoderFilterName = "AudioDecoderFilter";
        static const std::string AudioPlayerFilterName = "AudioPlayerFilter";
        static const std::string ImageConverterFilterName = "ImageConverterFilter";
        static const std::string VideoAnalysisFilterName = "VideoAnalysisFilter";
        static const std::string PictureEncoderFilterName = "PictureEncoderFilter";
        static const std::string AVTargetFilterName = "AVTargetFilter";

        //针脚名称
        static const std::string AVInputPinName = "AVInputPin";
        static const std::string AVOutputPinName = "AVOutputPin";
    }//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_GRAPH_DEFS_H