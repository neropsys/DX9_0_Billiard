#include "CText.h"
#include <windef.h>
CText::CText(){}
CText::~CText(){
	if (ppFont != NULL){
		ppFont->Release();
		ppFont = NULL;
	}
}

bool CText::create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight){
	rct.left = 512;
	rct.right = screenWidth;
	rct.top = 384;
	rct.bottom = screenHeight;
	if (FAILED(D3DXCreateFont(pDevice, 25, 12, 500, 0, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, 0, 0, "Times New Roman", &ppFont))){
		return false;
	}
	return true;
}
void CText::draw(LPCSTR pString, float x, float y){
	rct.left = x;
	rct.top = y;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	ppFont->DrawTextA(0, pString, -1, &rct, DT_LEFT | DT_TOP, fontColor);
}
void CText::destroy(){
	if (ppFont != NULL){
		ppFont->Release();
		ppFont = NULL;
	}
}