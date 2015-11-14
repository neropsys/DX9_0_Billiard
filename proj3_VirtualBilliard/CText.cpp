#include "CText.h"
#include <windef.h>
CText::CText(){}
CText::~CText(){
}

bool CText::create(IDirect3DDevice9* pDevice, int screenWidth, int screenHeight){
	rct.left = 0;
	rct.right = screenWidth;
	rct.top = 0;
	rct.bottom = screenHeight;
	fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (FAILED(D3DXCreateFont(pDevice, 25, 12, 500, 0, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, 0, 0, "Times New Roman", &ppFont))){
		return false;
	}
	anchor = DT_LEFT | DT_TOP;
	return true;
}
void CText::draw(LPCSTR pString){
	ppFont->DrawTextA(0, pString, -1, &rct, anchor, fontColor);
}
void CText::destroy(){
	if (ppFont != NULL){
		ppFont->Release();
		ppFont = NULL;
	}
}