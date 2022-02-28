//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D字体
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_FONT_H
#define MODULE_AV_STREAM_D3D_FONT_H

#include <cstdint>
#include <d3d9.h>
#include "av/d3dx9/d3dx9core.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3DFont
			{
			public:
				D3DFont(void);
				~D3DFont(void);

			public:
				ID3DXFont* createNew(IDirect3DDevice9* device = nullptr);
				int text(
					const RECT& rect, 
					const char* text = nullptr,  
					ID3DXFont* font = nullptr, 
					const uint32_t color = D3DCOLOR_RGBA(255, 0, 0, 255));
			};//class D3DFont
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_FONT_H

