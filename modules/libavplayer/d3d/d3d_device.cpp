#include "error_code.h"
#include "d3d_vertex_buffer.h"
#include "d3d_device.h"
using namespace module::av::stream;

static const uint32_t gFunctionCodeOf420ps30[] =
{
	0xffff0300, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0300,
	0x00000004, 0x0000001c, 0x20000102, 0x000000c0, 0x0000006c, 0x00010003,
	0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00020003, 0x00000001,
	0x00000074, 0x00000000, 0x0000008c, 0x00000003, 0x00000001, 0x00000074,
	0x00000000, 0x00000094, 0x00000002, 0x00000001, 0x000000a0, 0x000000b0,
	0x78655455, 0x00657574, 0x000c0004, 0x00010001, 0x00000001, 0x00000000,
	0x78655456, 0x00657574, 0x78655459, 0x00657574, 0x6e617274, 0x72617073,
	0x00746e65, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x3f800000,
	0x00000000, 0x00000000, 0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369,
	0x74666f73, 0x29522820, 0x534c4820, 0x6853204c, 0x72656461, 0x6d6f4320,
	0x656c6970, 0x2e392072, 0x392e3432, 0x322e3934, 0x00373033, 0x05000051,
	0xa00f0001, 0x3f950a81, 0x3fcc4a9d, 0xbf5fcbb4, 0x00000000, 0x05000051,
	0xa00f0002, 0x3f950a81, 0xbec89507, 0xbf501eac, 0x3f081b65, 0x05000051,
	0xa00f0003, 0x3f950a81, 0x40011a54, 0xbf8af5f5, 0x00000000, 0x05000051,
	0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f,
	0x80000005, 0x90030000, 0x0200001f, 0x80010005, 0x90030001, 0x0200001f,
	0x80020005, 0x90030002, 0x0200001f, 0x90000000, 0xa00f0800, 0x0200001f,
	0x90000000, 0xa00f0801, 0x0200001f, 0x90000000, 0xa00f0802, 0x03000042,
	0x800f0000, 0x90e40002, 0xa0e40802, 0x02000001, 0x80040000, 0x80000000,
	0x03000042, 0x800f0001, 0x90e40000, 0xa0e40800, 0x04000004, 0x80090000,
	0x80000001, 0xa0640004, 0xa0250004, 0x03000008, 0x80010800, 0xa0e40001,
	0x80f80000, 0x03000042, 0x800f0001, 0x90e40001, 0xa0e40801, 0x02000001,
	0x80020000, 0x80000001, 0x03000009, 0x80020800, 0xa0e40002, 0x80e40000,
	0x03000008, 0x80040800, 0xa0e40003, 0x80f40000, 0x02000001, 0x80080800,
	0xa0000000, 0x0000ffff
};

static const uint32_t gFunctionCodeOf420ps20[] =
{
	0xffff0200, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0200,
	0x00000004, 0x0000001c, 0x20000102, 0x000000c0, 0x0000006c, 0x00010003,
	0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00020003, 0x00000001,
	0x00000074, 0x00000000, 0x0000008c, 0x00000003, 0x00000001, 0x00000074,
	0x00000000, 0x00000094, 0x00000002, 0x00000001, 0x000000a0, 0x000000b0,
	0x78655455, 0x00657574, 0x000c0004, 0x00010001, 0x00000001, 0x00000000,
	0x78655456, 0x00657574, 0x78655459, 0x00657574, 0x6e617274, 0x72617073,
	0x00746e65, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x3f800000,
	0x00000000, 0x00000000, 0x00000000, 0x325f7370, 0x4d00305f, 0x6f726369,
	0x74666f73, 0x29522820, 0x534c4820, 0x6853204c, 0x72656461, 0x6d6f4320,
	0x656c6970, 0x2e392072, 0x392e3432, 0x322e3934, 0x00373033, 0x05000051,
	0xa00f0001, 0x3f950a81, 0x00000000, 0x3fcc4a9d, 0xbf5fcbb4, 0x05000051,
	0xa00f0002, 0x3f950a81, 0xbec89507, 0xbf501eac, 0x3f081b65, 0x05000051,
	0xa00f0003, 0x3f950a81, 0x40011a54, 0x00000000, 0xbf8af5f5, 0x05000051,
	0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f,
	0x80000000, 0xb0030000, 0x0200001f, 0x80000000, 0xb0030001, 0x0200001f,
	0x80000000, 0xb0030002, 0x0200001f, 0x90000000, 0xa00f0800, 0x0200001f,
	0x90000000, 0xa00f0801, 0x0200001f, 0x90000000, 0xa00f0802, 0x03000042,
	0x800f0000, 0xb0e40000, 0xa0e40800, 0x03000042, 0x800f0001, 0xb0e40001,
	0xa0e40801, 0x03000042, 0x800f0002, 0xb0e40002, 0xa0e40802, 0x02000001,
	0x80080003, 0xa0000000, 0x02000001, 0x80020000, 0x80000001, 0x02000001,
	0x80040000, 0x80000002, 0x02000001, 0x80080000, 0xa0000004, 0x03000009,
	0x80010003, 0xa0e40001, 0x80e40000, 0x03000009, 0x80020003, 0xa0e40002,
	0x80e40000, 0x03000009, 0x80040003, 0xa0e40003, 0x80e40000, 0x02000001,
	0x800f0800, 0x80e40003, 0x0000ffff
};

D3DDevice::D3DDevice()
{}

D3DDevice::~D3DDevice()
{}

IDirect3DDevice9* D3DDevice::createNew(
	IDirect3D9* d3d9 /* = nullptr */, 
	const void* hwnd /* = nullptr */, 
	const uint32_t width /* = 0 */, 
	const uint32_t height /* = 0 */)
{
	IDirect3DDevice9* device{ nullptr };

	if (d3d9 && hwnd && 0 < width && 0 < height)
	{
		D3DPRESENT_PARAMETERS param{ 0 };
		param.hDeviceWindow = (HWND)hwnd;
		param.BackBufferWidth = width;
		param.BackBufferHeight = height;
		param.BackBufferFormat = D3DFMT_UNKNOWN;
		param.BackBufferCount = 2;
		param.Windowed = TRUE;
		param.SwapEffect = D3DSWAPEFFECT_DISCARD;
		D3DCAPS9 caps9{};
		DWORD flag{ D3DCREATE_SOFTWARE_VERTEXPROCESSING };

		if (SUCCEEDED(d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps9)))
		{
			if (caps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			{
				flag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}

			if (SUCCEEDED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, param.hDeviceWindow, flag, (D3DPRESENT_PARAMETERS*)&param, &device)))
			{
			}
		}
	}

	return device;
}

const uint32_t* D3DDevice::functioncode(IDirect3DDevice9* device /*= NULL*/)
{
	uint32_t* fc{ nullptr };

	if (device)
	{
		D3DCAPS9 d3dcaps9{};
		if (SUCCEEDED(device->GetDeviceCaps(&d3dcaps9)))
		{
			fc = const_cast<uint32_t*>(
				d3dcaps9.PixelShaderVersion >= D3DPS_VERSION(3, 0) ? gFunctionCodeOf420ps30 : gFunctionCodeOf420ps20);
		}
	}

	return fc;
}

int D3DDevice::setPixelShader(
	IDirect3DDevice9* device /* = nullptr */, 
	IDirect3DPixelShader9* shader /* = nullptr */)
{
	int ret{ device && shader ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (SUCCEEDED(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX3)) && SUCCEEDED(device->SetPixelShader(shader)))
		{
		}
	}

	return ret;
}

int D3DDevice::setTexture(
	IDirect3DDevice9* device /* = nullptr */, 
	ID3DXConstantTable* constant /* = nullptr */, 
	IDirect3DTexture9** textures /* = nullptr */)
{
	int ret{ device && constant && textures ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		const char* const names[] =
		{
			"YTextue", "UTextue", "VTextue"
		};

		UINT number{ 0 };
		D3DXCONSTANT_DESC desc{};

		for (int i = 0; i != 3; ++i)
		{
			D3DXHANDLE handle{ constant->GetConstantByName(nullptr, names[i]) };

			if (handle &&
				SUCCEEDED(constant->GetConstantDesc(handle, &desc, &number)) &&
				SUCCEEDED(device->SetTexture(desc.RegisterIndex, textures[i])))
			{
			}
			else
			{
				break;
			}
		}
	}

	return ret;
}

int D3DDevice::present(
	IDirect3DDevice9* device /* = nullptr */, 
	IDirect3DVertexBuffer9* vertexBuffer /* = nullptr */)
{
	int ret{ device && vertexBuffer ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (SUCCEEDED(device->BeginScene()) &&
			SUCCEEDED(device->SetStreamSource(0, vertexBuffer, 0, sizeof(D3DVertex))) &&
			SUCCEEDED(device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2)) && 
			SUCCEEDED(device->EndScene()))
		{
			if (SUCCEEDED(device->Present(nullptr, nullptr, nullptr, nullptr)))
			{
			}
		}
	}

	return ret;
}
