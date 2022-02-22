//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D视频图片
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_TEXTURE_H
#define MODULE_AV_STREAM_D3D_TEXTURE_H

#include <d3d9.h>
#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3DTexture
			{
			public:
				D3DTexture(void);
				~D3DTexture(void);

			public:
				IDirect3DTexture9* createNew(
					IDirect3DDevice9* device = nullptr, 
					const uint32_t width = 0, 
					const uint32_t height = 0);
				int draw(
					IDirect3DTexture9* texture = nullptr, 
					const void* data = nullptr, 
					const uint32_t width = 0,
					const uint32_t height = 0);
			};//class D3DTexture
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_TEXTURE_H

