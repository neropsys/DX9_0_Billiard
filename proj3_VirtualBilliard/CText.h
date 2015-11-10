#pragma once
#include <d3dx9.h>

class CText{
public:
	CText();
	~CText();
	bool create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight);
	void draw(LPCSTR pString, float x, float y);
	void destroy();
private:
	LPD3DXFONT ppFont;
	RECT rct;
};