#pragma once
#include <d3dx9.h>
#include <string>


class CText{
public:
	CText();
	~CText();
	bool create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight, std::string name);
	void draw();
	inline void setAnchor(DWORD anchor){ this->anchor = anchor; }
	void setPosition(int x, int y);
	inline void setColor(D3DCOLOR color){ this->fontColor = color; }
	inline void setStr(LPCSTR str){ name = str; }
	void destroy();
private:
	std::string name;
	D3DCOLOR fontColor;
	DWORD anchor;
	bool create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight);
	void draw(LPCSTR pString, float x, float y);
private:
	LPD3DXFONT ppFont;
	RECT rct;
};