//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D像素作色器
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_PIXEL_SHADER_H
#define MODULE_AV_STREAM_D3D_PIXEL_SHADER_H

#include <d3d9.h>
#include "av/d3dx9/D3DX9Shader.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3DPixelShader
			{
			public:
				D3DPixelShader(void);
				~D3DPixelShader(void);

			public:
				IDirect3DPixelShader9* createNew(IDirect3DDevice9* device = nullptr);
				ID3DXConstantTable* constanttable(IDirect3DDevice9* device = nullptr);
			};//class D3DPixelShader
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_PIXEL_SHADER_H

