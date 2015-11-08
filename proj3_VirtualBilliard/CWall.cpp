#include "CWall.h"

CWall::CWall(){
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_width = 0;
	m_depth = 0;
	m_pMesh = NULL;
}
CWall::~CWall(){}

bool CWall::create(IDirect3DDevice9* pDevice,
	float ix,
	float iz,
	float iwidth,
	float iheight,
	float idepth,
	D3DXCOLOR color){
	if (!CObject::create(pDevice, color))
		return false;

	m_width = iwidth;
	m_depth = idepth;
	
	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pMesh, NULL)))
		return false;
	return true;
}

void CWall::destroy(){
	if (m_pMesh != NULL){
		m_pMesh->Release();
		m_pMesh = NULL;
	}
		
}
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView){
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pMesh->DrawSubset(0);
}
bool CWall::hasIntersected(CSphere& ball){
	// Insert your code here.
	return false;
}
void CWall::hitBy(CSphere& ball){
	// Insert your code here.
}

void CWall::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
