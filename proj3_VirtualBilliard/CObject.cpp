#include "CObject.h"

CObject::CObject(){}
CObject::~CObject(){}
bool CObject::create(IDirect3DDevice9* pDevice, CObject::Shape shape){
	
	return false;
}
void CObject::setPosition(float x, float y, float z){

	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
void CObject::destroy(){

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_shaderCode != NULL){
		m_shaderCode->Release();
		m_shaderCode = NULL;
	}
	if (m_texture != NULL) {
		m_texture->Release();
		m_texture = NULL;
	}
	if (m_effect != NULL){
		m_effect->Release();
		m_effect = NULL;
	}
}