//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-24
//		Description : D3D画线
//
//		History:
//					1. 2022-02-24 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_LINE_H
#define MODULE_AV_STREAM_D3D_LINE_H

#include <cstdint>
#include <d3d9.h>
#include "av/d3dx9/d3dx9core.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3DLine
			{
			public:
				D3DLine(void);
				~D3DLine(void);

			public:
				ID3DXLine* createNew(IDirect3DDevice9* device = nullptr);
				int draw(
					const RECT& rect, 
					ID3DXLine* line = nullptr, 
					const uint16_t lw = 5, 
					const uint32_t color = D3DCOLOR_RGBA(255, 0, 0, 255));
			};//class D3DLine
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_LINE_H

