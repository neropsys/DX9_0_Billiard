#include "CCue.h"
#include "ConstVariable.h"
CCue::CCue(){ D3DXMatrixIdentity(&m_mLocal); }
CCue::~CCue(){}
bool CCue::create(IDirect3DDevice9* pDevice){
	if (CObject::create(pDevice, Shape::CYLINDER) == false) return false;
	
	LPD3DXMESH newMesh = convertMesh(pDevice, m_pMesh);
	if (newMesh == nullptr) return false;

	effectFile = CYLINDER_EFFECT;
	textureFile = CYLINDER_TEXTURE;


	m_texture = LoadTexture(pDevice, textureFile);
	m_effect = LoadShader(pDevice, effectFile);

	m_pMesh->Release();
	m_pMesh = newMesh;
	return true;
}
void CCue::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView){
	if (NULL == pDevice)
		return;
	//pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	//pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	//m_pMesh->DrawSubset(0);
	//return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	D3DXMATRIX proj;
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	m_effect->SetMatrix("gLocalMatrix", &m_mLocal);
	m_effect->SetMatrix("gWorldMatrix", &mWorld);
	m_effect->SetMatrix("gViewMatrix", &mView);
	//m_effect->SetVector("gWorldLightPosition", &mLightPos);
	m_effect->SetMatrix("gProjectionMatrix", &proj);
	m_effect->SetTexture("DiffuseMap", m_texture);
	//m_effect->

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_effect->SetTechnique("ColorShader");
	UINT numPass = 0;
	m_effect->Begin(&numPass, NULL);
	{
		for (UINT i = 0; i < numPass; ++i)
		{
			m_effect->BeginPass(i);
			m_pMesh->DrawSubset(0);
			m_effect->EndPass();
		}
	}
	m_effect->End();

}
void CCue::playHit(){

}
LPD3DXMESH CCue::convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh){


	LPD3DXMESH newMesh = nullptr;
	VERTEX* pVerts;
	HRESULT result = mesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);
	
	if (FAILED(result)) return nullptr;
	
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			D3DXVECTOR3 v = pVerts->pos - getPosition();
			D3DXVec3Normalize(&v, &v);
			pVerts->tu = asin(v.x) / D3DX_PI + .5f;
			pVerts->tv = asin(v.y) / D3DX_PI + .5f;

			pVerts->pos.z += 3.f;

			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
		//temporary uv generator
		return newMesh;
	}
	else{	
		return nullptr;
	}

}