#include "error_code.h"
#include "d3d_texture.h"
using namespace module::av::stream;

D3DTexture::D3DTexture()
{}

D3DTexture::~D3DTexture()
{}

IDirect3DTexture9* D3DTexture::createNew(
	IDirect3DDevice9* device /* = nullptr */, 
	const uint32_t width /* = 0 */, 
	const uint32_t height /* = 0 */)
{
	IDirect3DTexture9* texture{ NULL };

	if (device && 0 < width && 0 < height)
	{
		if (SUCCEEDED(device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &texture, NULL)))
		{
		}
	}

	return texture;
}

int D3DTexture::draw(
	IDirect3DTexture9* texture /* = nullptr */, 
	const void* data /* = nullptr */, 
	const uint32_t width /* = 0 */, 
	const uint32_t height /* = 0 */)
{
	int ret{ texture && data && 0 < width && 0 < height ? Error_Code_Success : Error_Code_Invalid_Param };
	D3DLOCKED_RECT rect;

	if (SUCCEEDED(texture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD)))
	{
		const uint32_t area{ width * height };

		if (rect.Pitch == width)
		{
			memcpy_s(rect.pBits, area, data, area);
		}
		else
		{
			int pos{ 0 }, times{ 0 };

			for (int i = 0; i != height; ++i)
			{
				memcpy_s((uint8_t*)rect.pBits + pos, width, (uint8_t*)data + times * width, width);
				pos += rect.Pitch;
				++times;
			}
		}

		texture->UnlockRect(0);
	}
	else
	{
		ret = Error_Code_Operate_Failure;
	}

	return ret;
}
