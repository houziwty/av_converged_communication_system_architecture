#include "error_code.h"
#include "d3d_line.h"
using namespace module::av::stream;

D3DLine::D3DLine()
{}

D3DLine::~D3DLine()
{}

ID3DXLine* D3DLine::createNew(IDirect3DDevice9* device /* = NULL */)
{
	ID3DXLine* line{ NULL };

	if (device)
	{
		if (SUCCEEDED(D3DXCreateLine(device, &line)))
		{
		}
	}

	return line;
}

int D3DLine::draw(
	const RECT& rect, 
	ID3DXLine* line /* = nullptr */, 
	const uint32_t color /* = D3DCOLOR_RGBA(255, 0, 0, 255) */)
{
	int ret{ line ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		FLOAT x{ (FLOAT)rect.left }, y{ (FLOAT)rect.top }, w{ (FLOAT)(rect.right - rect.left) }, h{ (FLOAT)(rect.bottom - rect.top) };
		D3DXVECTOR2 vertex[5]{ 
			{x, y},
			{x + w, y},
			{x+ w, y + h},
			{x, y + h},
			{x, y} };
		line->SetWidth(5);
		ret = (S_OK == line->Draw(vertex, 5, color) ? ret : Error_Code_Operate_Failure);
	}

	return ret;
}
