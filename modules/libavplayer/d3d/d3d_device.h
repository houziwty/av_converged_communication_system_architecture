//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D设备
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_DEVICE_H
#define MODULE_AV_STREAM_D3D_DEVICE_H

#include <d3d9.h>
#include "av/d3dx9/d3dx9shader.h"
#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3DDevice
			{
			public:
				D3DDevice(void);
				~D3DDevice(void);

			public:
				IDirect3DDevice9* createNew(
					IDirect3D9* d3d9 = nullptr, 
					const void* hwnd = nullptr, 
					const uint32_t width = 0, 
					const uint32_t height = 0);
				const uint32_t* functioncode(IDirect3DDevice9* device = nullptr);
				int setPixelShader(
					IDirect3DDevice9* device = nullptr, 
					IDirect3DPixelShader9* shader = nullptr);
				int setTexture(
					IDirect3DDevice9* device = nullptr,
					ID3DXConstantTable* constant = nullptr,
					IDirect3DTexture9** textures = nullptr);
				int present(
					IDirect3DDevice9* device = nullptr, 
					IDirect3DVertexBuffer9* vertexBuffer = nullptr);
			};//class D3DDevice
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_DEVICE_H
