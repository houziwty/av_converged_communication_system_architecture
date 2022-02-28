#include "av_pkt.h"
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
	void* areas /* = nullptr */)
	: AVPlayer(id), displayHwnd{ hwnd }, drawAreaParam{areas}, d3d9{ nullptr }, d3d9Device{ nullptr },
	d3d9PixelShader{ nullptr }, d3d9VertexBuffer{ nullptr }, d3dxFont{ nullptr }, d3dxLine{nullptr}
{
	d3d9Texture[0] = d3d9Texture[1] = d3d9Texture[2] = nullptr;
}

D3D::~D3D()
{
	destroy();
}

int D3D::input(const AVPkt* avpkt /* = nullptr */)
{
	int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (!d3d9 && !d3d9Device && !d3d9PixelShader && !d3d9VertexBuffer)
		{
			ret = createNew(1920, 1080);
		}

		ret = (Error_Code_Success == ret ? draw(avpkt->data(), 1920, 1080) : Error_Code_Bad_New_Object);
	}

	return ret;
}

int D3D::createNew(const uint32_t width /* = 0 */, const uint32_t height /* = 0 */)
{
	int ret{ Error_Code_Bad_New_Object };
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	d3d9Device = D3DDevice().createNew(d3d9, displayHwnd, width, height);
	d3d9PixelShader = D3DPixelShader().createNew(d3d9Device);
	d3d9VertexBuffer = D3DVertexBuffer().createNew(d3d9Device, width, height);
	d3d9Texture[0] = D3DTexture().createNew(d3d9Device, width, height);
	d3d9Texture[1] = D3DTexture().createNew(d3d9Device, width / 2, height / 2);
	d3d9Texture[2] = D3DTexture().createNew(d3d9Device, width / 2, height / 2);
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
		}

		ret = Error_Code_Success;
	}
	else
	{
		destroy();
	}

	return ret;
}

int D3D::destroy(void)
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

int D3D::draw(
	const void* data /* = nullptr */, 
	const int width /* = 0 */, 
	const int height /* = 0 */)
{
	int u_pos{ width * height }, v_pos{ u_pos * 5 / 4 };

	d3d9Device->BeginScene();
	d3d9Device->SetStreamSource(0, d3d9VertexBuffer, 0, sizeof(D3DVertex));
	d3d9Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	D3DTexture().draw(d3d9Texture[0], data, width, height);
	D3DTexture().draw(d3d9Texture[1], (uint8_t*)data + u_pos, width / 2, height / 2);
	D3DTexture().draw(d3d9Texture[2], (uint8_t*)data + v_pos, width / 2, height / 2);

	if (drawAreaParam)
	{
		std::vector<AVDrawArea>* temp_areas{
			reinterpret_cast<std::vector<AVDrawArea>*>(drawAreaParam) };
		std::vector<AVDrawArea> areas{ *temp_areas };
		const std::size_t number{ areas.size() };

		for (int i = 0; i != number; ++i)
		{
			RECT rc{
				areas[i].left,
				areas[i].top,
				areas[i].right,
				areas[i].bottom };
			D3DLine().draw(rc, d3dxLine, D3DCOLOR_RGBA(areas[i].color[0], areas[i].color[1], areas[i].color[2], 0xFF));
			D3DFont().text(rc, areas[i].text, d3dxFont, D3DCOLOR_RGBA(areas[i].color[0], areas[i].color[1], areas[i].color[2], 0xFF));
		}
	}

	d3d9Device->EndScene();
	d3d9Device->Present(nullptr, nullptr, nullptr, nullptr);

	return Error_Code_Success;
}
