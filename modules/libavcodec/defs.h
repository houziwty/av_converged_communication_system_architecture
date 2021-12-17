//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : AVStreamCodec库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_AV_STREAM_CODEC_DEFS_H
#define MODULE_AV_STREAM_CODEC_DEFS_H

#if defined(WINDOWS)
#ifdef USE_MODULE_AV_CODEC
#define AV_CODEC_EXPORT __declspec(dllimport)
#else
#define AV_CODEC_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_CODEC
#elif defined(__linux__)
#define AV_CODEC_EXPORT
#endif//WINDOWS

namespace module
{
	namespace avstream
	{
    }//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_CODEC_DEFS_H