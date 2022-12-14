#include "libavpkt.h"
#include "error_code.h"
#include "d3d_device.h"
#include "d3d_pixel_shader.h"
#include "d3d_vertex_buffer.h"
#include "d3d_texture.h"
#include "d3d_font.h"
#include "d3d_line.h"
#include "d3d.h"
using namespace module::av::stream;

D3D::D3D(
	const uint32_t id /* = 0 */, 
	void* hwnd /* = nullptr */, 
	ExtendDrawInfo* areas /* = nullptr */)
	: AVPlayerNode(id), displayHwnd{ hwnd }, extendDrawInfo{areas}, d3d9{ nullptr }, d3d9Device{ nullptr },
	d3d9PixelShader{ nullptr }, d3d9VertexBuffer{ nullptr }, d3dxFont{ nullptr }, d3dxLine{nullptr},
	width{ 0 }, height{ 0 }
{
	d3d9Texture[0] = d3d9Texture[1] = d3d9Texture[2] = nullptr;
}

D3D::~D3D()
{
	uninit();
}

int D3D::input(const void* avpkt /* = nullptr */)
{
	int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret && Error_Code_Success == init(avpkt))
	{
		ret = draw(avpkt);
	}

	return ret;
}

int D3D::init(const void* avpkt /* = nullptr */)
{
	int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
		const uint32_t w{ pkt->width() }, h{ pkt->height() };

		if (0 < w && 0 < h)
		{
			if ((!d3d9 && !d3d9Device) || (width != w && height != h))
			{
				uninit();
				d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
				d3d9Device = D3DDevice().createNew(d3d9, displayHwnd, w, h);
				d3d9PixelShader = D3DPixelShader().createNew(d3d9Device);
				d3d9VertexBuffer = D3DVertexBuffer().createNew(d3d9Device, w, h);
				d3d9Texture[0] = D3DTexture().createNew(d3d9Device, w, h);
				d3d9Texture[1] = D3DTexture().createNew(d3d9Device, w / 2, h / 2);
				d3d9Texture[2] = D3DTexture().createNew(d3d9Device, w / 2, h / 2);
				d3dxFont = D3DFont().createNew(d3d9Device);
				d3dxLine = D3DLine().createNew(d3d9Device);

				if (d3d9 && d3d9Device && d3d9PixelShader && d3d9VertexBuffer && d3dxFont &&
					d3d9Texture[0] && d3d9Texture[1] && d3d9Texture[2])
				{
					ID3DXConstantTable* table{ D3DPixelShader().constanttable(d3d9Device) };
					if (table)
					{
						D3DDevice().setPixelShader(d3d9Device, d3d9PixelShader);
						D3DDevice().setTexture(d3d9Device, table, d3d9Texture);
						table->Release();

						width = w;
						height = h;
					}
				}
			}
		}
	}

	return d3d9 && d3d9Device && d3d9PixelShader && d3d9VertexBuffer && d3dxFont &&
		d3d9Texture[0] && d3d9Texture[1] && d3d9Texture[2] ? Error_Code_Success : Error_Code_Operate_Failure;
}

int D3D::uninit(void)
{
	for (int i = 0; i != 3; ++i)
	{
		if (d3d9Texture[i])
		{
			d3d9Texture[i]->Release();
		}
	}

	if (d3d9VertexBuffer)
	{
		d3d9VertexBuffer->Release();
	}

	if (d3d9PixelShader)
	{
		d3d9PixelShader->Release();
	}
	
	if (d3dxFont)
	{
		d3dxFont->Release();
	}

	if (d3dxLine)
	{
		d3dxLine->Release();
	}

	if (d3d9Device)
	{
		d3d9Device->Release();
	}
	
	if (d3d9)
	{
		d3d9->Release();
	}

	return Error_Code_Success;
}

int D3D::draw(const void* avpkt /* = nullptr */)
{
	uint32_t u_pos{ width * height }, v_pos{ u_pos * 5 / 4 };
	Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
	const uint8_t* data{ reinterpret_cast<const uint8_t*>(pkt->data()) };

	d3d9Device->BeginScene();
	d3d9Device->SetStreamSource(0, d3d9VertexBuffer, 0, sizeof(D3DVertex));
	d3d9Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	D3DTexture().draw(d3d9Texture[0], data, width, height);
	D3DTexture().draw(d3d9Texture[1], (uint8_t*)data + u_pos, width / 2, height / 2);
	D3DTexture().draw(d3d9Texture[2], (uint8_t*)data + v_pos, width / 2, height / 2);

	if (extendDrawInfo && extendDrawInfo->enable && extendDrawInfo->areas)
	{
		for (int i = 0; i != extendDrawInfo->number; ++i)
		{
			RECT rc{
				extendDrawInfo->areas[i].left,
				extendDrawInfo->areas[i].top,
				extendDrawInfo->areas[i].right,
				extendDrawInfo->areas[i].bottom };
			D3DLine().draw(
				rc, d3dxLine, extendDrawInfo->areas[i].lw, D3DCOLOR_RGBA(extendDrawInfo->areas[i].color[0], extendDrawInfo->areas[i].color[1], extendDrawInfo->areas[i].color[2], 0xFF));
			D3DFont().text(
				rc, extendDrawInfo->areas[i].text, d3dxFont, D3DCOLOR_RGBA(extendDrawInfo->areas[i].color[0], extendDrawInfo->areas[i].color[1], extendDrawInfo->areas[i].color[2], 0xFF));
		}
	}

	d3d9Device->EndScene();
	d3d9Device->Present(nullptr, nullptr, nullptr, nullptr);

	return Error_Code_Success;
}
