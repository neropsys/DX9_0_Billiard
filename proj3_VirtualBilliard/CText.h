#pragma once
#include <d3dx9.h>

class CText{
public:
	CText();
	~CText();
	bool create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight);
	void draw(LPCSTR pString);
	inline void setAnchor(DWORD anchor){ this->anchor = anchor; }
	inline void setColor(D3DCOLOR color){ this->fontColor = color; }
	void destroy();
private:
	D3DCOLOR fontColor;
	DWORD anchor;
	LPD3DXFONT ppFont;
	RECT rct;
};