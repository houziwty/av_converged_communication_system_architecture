#include "error_code.h"
#include "d3d_font.h"
using namespace module::av::stream;

D3DFont::D3DFont()
{}

D3DFont::~D3DFont()
{}

ID3DXFont* D3DFont::createNew(IDirect3DDevice9* device /* = NULL */)
{
	ID3DXFont* font{ NULL };

	if (device)
	{
		if (SUCCEEDED(D3DXCreateFontA(device, 35, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font)))
		{
		}
	}

	return font;
}

int D3DFont::text(
	const RECT& rect, 
	const char* text /* = nullptr */, 
	ID3DXFont* font /* = nullptr */)
{
	int ret{ text && font ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		ret = (0 <= font->DrawTextA(NULL, text, -1, (RECT*)&rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 0, 0, 255)) ? Error_Code_Success : Error_Code_Operate_Failure);
	}

	return ret;
}
