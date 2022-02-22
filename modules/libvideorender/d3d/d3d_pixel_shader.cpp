#include "d3d_device.h"
#include "d3d_pixel_shader.h"
using namespace module::av::stream;

D3DPixelShader::D3DPixelShader()
{}

D3DPixelShader::~D3DPixelShader()
{}

IDirect3DPixelShader9* D3DPixelShader::createNew(IDirect3DDevice9* device /* = nullptr */)
{
	IDirect3DPixelShader9* pixelshader{ nullptr };
	const uint32_t* fc{ D3DDevice().functioncode(device) };

	if (device && fc)
	{
		if (SUCCEEDED(device->CreatePixelShader((const DWORD*)fc, &pixelshader)))
		{
		}
	}

	return pixelshader;
}

ID3DXConstantTable* D3DPixelShader::constanttable(IDirect3DDevice9* device /* = nullptr */)
{
	ID3DXConstantTable* table{ nullptr };
	const uint32_t* fc{ D3DDevice().functioncode(device) };

	if (device && fc)
	{
		if (SUCCEEDED(D3DXGetShaderConstantTable((const DWORD*)fc, &table)) &&
			SUCCEEDED(table->SetDefaults(device)))
		{
		}
	}

	return table;
}
