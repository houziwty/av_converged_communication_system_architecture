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
#ifdef USE_MODULE_AV_STREAM
#define AV_STREAM_EXPORT __declspec(dllimport)
#else
#define AV_STREAM_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_STREAM
#elif defined(__linux__)
#define AV_STREAM_EXPORT
#endif//_WINDOWS

//AV流图角色配置
typedef struct tagAVModeConf_t
{
    uint32_t avid;                   //>0，由调用者分配
    void* hwnd;                      //视频播放窗口
}AVModeConf;

#endif//MODULE_AV_STREAM_AV_STREAM_DEFS_H