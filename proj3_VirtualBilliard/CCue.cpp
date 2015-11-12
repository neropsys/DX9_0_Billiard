#include "CCue.h"

CCue::CCue(){}
CCue::~CCue(){}
bool CCue::create(IDirect3DDevice9* pDevice){
//	m_pMesh = createMesh(pDevice);
	if (m_pMesh == nullptr) return false;
	
//	m_effect = LoadShader(pDevice, FILENAME);

	if (m_effect == nullptr) return false;

	return true;

}

void CCue::destroy(){

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