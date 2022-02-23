#include "d3d_vertex_buffer.h"
using namespace module::av::stream;

D3DVertexBuffer::D3DVertexBuffer()
{}

D3DVertexBuffer::~D3DVertexBuffer()
{}

IDirect3DVertexBuffer9* D3DVertexBuffer::createNew(
	IDirect3DDevice9* device /* = nullptr */, 
	const uint32_t width /* = 0 */, 
	const uint32_t height /* = 0 */)
{
	IDirect3DVertexBuffer9* vb{ nullptr };

	if (device && 0 < width && 0 < height)
	{
		if (SUCCEEDED(device->CreateVertexBuffer(
			3 * sizeof(D3DVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX3, D3DPOOL_DEFAULT, &vb, NULL)))
		{
			D3DVertex* vertex{ nullptr };

			if (SUCCEEDED(vb->Lock(0, 0, (void**)&vertex, 0)))
			{
				long x = 0;
				long y = 0;
				long dx = width;
				long dy = height;
				vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

				vertex[0].pos = D3DXVECTOR4(x - 0.5f, y - 0.5f, 0.0f, 1.0f);
				vertex[1].pos = D3DXVECTOR4(dx - 0.5f, y - 0.5f, 0.0f, 1.0f);
				vertex[2].pos = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
				vertex[3].pos = D3DXVECTOR4(x - 0.5f, dy - 0.5f, 0.0f, 1.0f);

				//				if (m_Flip == ID3DVRInterface::Not_Upper_Down_Flip)
				//				{
				vertex[0].tex1 = vertex[0].tex2 = vertex[0].tex3 = D3DXVECTOR2(0.0f, 0.0f);
				vertex[1].tex1 = vertex[1].tex2 = vertex[1].tex3 = D3DXVECTOR2(1.0f, 0.0f);
				vertex[2].tex1 = vertex[2].tex2 = vertex[2].tex3 = D3DXVECTOR2(1.0f, 1.0f);
				vertex[3].tex1 = vertex[3].tex2 = vertex[3].tex3 = D3DXVECTOR2(0.0f, 1.0f);
				//				}
				// 				else
				// 				{
				// 					direct3DVertex[0].tex1 = direct3DVertex[0].tex2 = direct3DVertex[0].tex3 = D3DXVECTOR2(0.0f, 1.0f);
				// 					direct3DVertex[1].tex1 = direct3DVertex[1].tex2 = direct3DVertex[1].tex3 = D3DXVECTOR2(1.0f, 1.0f);
				// 					direct3DVertex[2].tex1 = direct3DVertex[2].tex2 = direct3DVertex[2].tex3 = D3DXVECTOR2(1.0f, 0.0f);
				// 					direct3DVertex[3].tex1 = direct3DVertex[3].tex2 = direct3DVertex[3].tex3 = D3DXVECTOR2(0.0f, 0.0f);
				// 				}

				vb->Unlock();
			}
		}
	}

	return vb;
}
