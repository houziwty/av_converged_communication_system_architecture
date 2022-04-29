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
#include <functional>

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV
#define AV_EXPORT __declspec(dllimport)
#else
#define AV_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV
#elif defined(__linux__)
#define AV_EXPORT
#endif//_WINDOWS

//帧数据回调函数
//@_1 [out] : 模块ID标识
//@_2 [out] : AVPkt帧实例
typedef std::function<void(const uint32_t, const void*)> AVFrameDataCallback;

//AV流图类型
typedef enum class tagAVModeType_t : uint32_t
{
    AV_MODE_TYPE_NONE = 0, 
    AV_MODE_TYPE_GRAB_PS, 
    AV_MODE_TYPE_GRAB_BRG24, 
    AV_MODE_TYPE_GRAB_RGB24, 
    AV_MODE_TYPE_GRAB_JPEG, 
    AV_MODE_TYPE_REALPLAY, 
    AV_MODE_TYPE_PLAYBACK, 
    AV_MODE_TYPE_DOWNLOAD
}AVModeType;

//绘制参数
typedef struct tagAVDrawParam_t
{
	uint32_t left;
	uint32_t top;
	uint32_t right;
	uint32_t bottom;
	uint8_t color[3];                   //RGB颜色值，0-R,1-G,2-B
	char text[256];
    uint16_t lw;				        //线宽
}AVDrawParam;

//用户绘制参数
typedef struct tagExtendDrawInfo_t
{
    bool enable;
    uint32_t number;
    AVDrawParam* areas;
}ExtendDrawInfo;

//AV流图角色配置
typedef struct tagAVModeConf_t
{
    uint32_t id;                      //>0，由调用者分配
    AVModeType type;
    void* hwnd;                       //视频播放窗口
    ExtendDrawInfo* infos;            //仅当AV_MODE_TYPE_STREAM_PLAY == type时有效
    AVFrameDataCallback callback;     //仅当AV_MODE_TYPE_GRAB_* == type时有效
}AVModeConf;

//AV过滤器名称
static const char* av_buffer_parser_filter_name = "av_buffer_parser_filter";
static const char* av_frame_parser_filter_name = "av_frame_parser_filter";
static const char* av_frame_decoder_filter_name = "av_frame_decoder_filter";
static const char* av_frame_encoder_filter_name = "av_frame_encoder_filter";
static const char* av_frame_converter_filter_name = "av_frame_converter_filter";
static const char* av_frame_player_filter_name = "av_frame_player_filter";

//AV针脚名称
static const char* av_input_pin_name = "av_input_pin";
static const char* av_output_pin_name = "av_output_pin";

#endif//MODULE_AV_STREAM_AV_STREAM_DEFS_H