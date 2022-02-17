//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-14
//		Description : AV流图定义
//
//		History:
//					1. 2022-02-14 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_STREAM_DEFS_H
#define MODULE_AV_STREAM_AV_STREAM_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV
#define AV_EXPORT __declspec(dllimport)
#else
#define AV_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV
#elif defined(__linux__)
#define AV_EXPORT
#endif//_WINDOWS

//AV流图类型
typedef enum class tagAVModeType_t : uint32_t
{
    AV_MODE_TYPE_NONE = 0,
    AV_MODE_TYPE_ENCODE, 
    AV_MODE_TYPE_DECODE, 
    AV_MODE_TYPE_RENDER
}AVModeType;

//AV流图角色配置
typedef struct tagAVModeConf_t
{
    uint32_t id;                      //>0，由调用者分配
    AVModeType type;
    void* hwnd;                       //视频播放窗口
}AVModeConf;

//AV过滤器名称
static const char* av_buffer_parser_filter_name = "av_buffer_parser_filter";
static const char* av_ps_parser_filter_name = "av_ps_parser_filter";
static const char* av_video_decoder_filter_name = "av_video_decoder_filter";
static const char* av_image_converter_filter_name = "av_image_converter_filter";
static const char* av_video_render_filter_name = "av_video_render_filter";
static const char* av_data_callback_filter_name = "av_data_callback_filter";

//AV针脚名称
static const char* av_video_input_pin_name = "av_video_input_pin";
static const char* av_video_output_pin_name = "av_video_output_pin";
static const char* av_audio_input_pin_name = "av_audio_input_pin";
static const char* av_audio_output_pin_name = "av_audio_output_pin";

#endif//MODULE_AV_STREAM_AV_STREAM_DEFS_H