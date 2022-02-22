//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D视频显示器
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_D3D_VIDEO_RENDER_H
#define MODULE_AV_STREAM_D3D_VIDEO_RENDER_H

#include <d3d9.h>
#include "av/d3dx9/d3dx9core.h"
#include "video_render.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class D3D : public VideoRender
			{
			public:
				D3D(void* hwnd = nullptr, const uint32_t id = 0);
				~D3D(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;

			private:
				int createNew(const uint32_t width = 0, const uint32_t height = 0);
				int destroy(void);
				int draw(
					const void* data = nullptr, const int width = 0, const int height = 0);
				int text(const RECT& rect, const char* text = nullptr);

			private:
				const void* displayHwnd;
				IDirect3D9* d3d9;
				IDirect3DDevice9* d3d9Device;
				IDirect3DPixelShader9* d3d9PixelShader;
				IDirect3DVertexBuffer9* d3d9VertexBuffer;
				IDirect3DTexture9* d3d9Texture[3];
				ID3DXFont* d3dxFont;
				ID3DXLine* d3dxLine;
			};//class D3D
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_VIDEO_RENDER_H
