//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : ������
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : D3D��Ƶ��ʾ��
//
//		History:
//					1. 2022-02-21 ������������
//

#ifndef MODULE_AV_STREAM_D3D_VIDEO_RENDER_H
#define MODULE_AV_STREAM_D3D_VIDEO_RENDER_H

#include <d3d9.h>
#include "av/d3dx9/d3dx9core.h"
#include "av_player_node.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			//绘制参数
			typedef struct tagAVDrawParam_t
			{
				uint32_t left;
				uint32_t top;
				uint32_t right;
				uint32_t bottom;
				uint8_t color[3];                   //RGB颜色值，0-R,1-G,2-B
				char text[256];
			}AVDrawParam;

			//用户绘制参数
			typedef struct tagExtendDrawInfo_t
			{
				bool enable;
				uint32_t number;
				AVDrawParam* areas;
			}ExtendDrawInfo;

			class D3D : public AVPlayerNode
			{
			public:
				D3D(
					const uint32_t id = 0, 
					void* hwnd = nullptr, 
					ExtendDrawInfo* areas = nullptr);
				~D3D(void);

			public:
				int input(const void* avpkt = nullptr) override;

			private:
				int init(const void* avpkt = nullptr);
				int uninit(void);
				int draw(const void* avpkt = nullptr);

			private:
				const void* displayHwnd;
				ExtendDrawInfo* extendDrawInfo;
				IDirect3D9* d3d9;
				IDirect3DDevice9* d3d9Device;
				IDirect3DPixelShader9* d3d9PixelShader;
				IDirect3DVertexBuffer9* d3d9VertexBuffer;
				IDirect3DTexture9* d3d9Texture[3];
				ID3DXFont* d3dxFont;
				ID3DXLine* d3dxLine;
				uint32_t width;
				uint32_t height;
			};//class D3D
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_D3D_VIDEO_RENDER_H
