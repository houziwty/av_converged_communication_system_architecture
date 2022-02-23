//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D顶点缓存
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_VERTEX_BUFFER_H
#define MODULE_AV_STREAM_D3D_VERTEX_BUFFER_H

#include <d3d9.h>
#include "av/d3dx9/d3dx9math.h"
#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			typedef struct tagD3DVertex_t
			{
				D3DXVECTOR4 pos;
				DWORD color;
				D3DXVECTOR2 tex1;
				D3DXVECTOR2 tex2;
				D3DXVECTOR2 tex3;
			}D3DVertex;

			class D3DVertexBuffer
			{
			public:
				D3DVertexBuffer(void);
				~D3DVertexBuffer(void);

			public:
				IDirect3DVertexBuffer9* createNew(
					IDirect3DDevice9* device = nullptr, 
					const uint32_t width = 0, 
					const uint32_t height = 0);
			};//class D3DVertexBuffer
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_VERTEX_BUFFER_H

