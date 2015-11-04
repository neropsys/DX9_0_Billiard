#include "CObject.h"

CObject::CObject(){}
CObject::~CObject(){}
bool CObject::create(IDirect3DDevice9* pDevice, D3DXCOLOR color){
	if (NULL == pDevice)
		return false;
	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	return true;
}