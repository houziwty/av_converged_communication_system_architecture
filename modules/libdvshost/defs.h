//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : DVS库定义
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DVS_HOST_DEFS_H
#define MODULE_DVS_HOST_DEFS_H

#if defined(WINDOWS)
#ifdef USE_MODULE_DVS_HOST
#define DVS_HOST_EXPORT extern "C" __declspec(dllimport)
#else
#define DVS_HOST_EXPORT extern "C" __declspec(dllexport)
#endif//USE_MODULE_DVS_HOST
#elif defined(__linux__)
#define DVS_HOST_EXPORT
#endif//WINDOWS

#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"

typedef enum class tagFactoryType_t : int
{
	FACTORY_TYPE_NONE = 0,
	FACTORY_TYPE_HK
}FactoryType;

typedef enum class tagCameraType_t : int
{
	CAMERA_TYPE_ANALOGY = 0,
	CAMERA_TYPE_DIGITAL
}CameraType;

typedef enum class tagStreamType_t : int
{
	STREAM_TYPE_NONE = 0,
	STREAM_TYPE_REALPLAY,
	STREAM_TYPE_PLAYBACK,
	STREAM_TYPE_DOWNLOAD
}StreamType;

namespace module
{
	namespace dvs
	{
		class Device;
		class Camera;
		class Stream;

		using DevicePtr = boost::shared_ptr<Device>;
		using CameraPtr = boost::shared_ptr<Camera>;
		using CameraPtrs = std::vector<CameraPtr>;
		using StreamPtr = boost::shared_ptr<Stream>;
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_DEFS_H