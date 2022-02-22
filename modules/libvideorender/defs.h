//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : 视频显示库定义
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_VIDEO_RENDER_DEFS_H
#define MODULE_AV_STREAM_VIDEO_RENDER_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_VIDEO_RENDER
#define VIDEO_RENDER_EXPORT __declspec(dllimport)
#else
#define VIDEO_RENDER_EXPORT __declspec(dllexport)
#endif//USE_MODULE_VIDEO_RENDER
#elif defined(__linux__)
#define VIDEO_RENDER_EXPORT
#endif//_WINDOWS

typedef struct tagVideoRenderModeConf_t
{
	uint32_t id;                   //>0，由调用者分配
	void* hwnd;
}VideoRenderModeConf;

#endif//MODULE_AV_STREAM_VIDEO_RENDER_DEFS_H