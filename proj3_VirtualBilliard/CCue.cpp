#include "CCue.h"
#include "ConstVariable.h"
CCue::CCue(){
	D3DXMatrixIdentity(&m_mLocal);
	animationInit = false;
	moveDistance = 0.f;
	pulled = false;
	playing = false;
	isVisible = true;
}
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
void CCue::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView, float time){
	if (NULL == pDevice)
		return;
	//pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	//pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	//m_pMesh->DrawSubset(0);
	//return;
	if (!isVisible) return;
	if (animationInit){
		playAnimation(time);
	}
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
	animationInit = true;
	playing = true;
}
void CCue::playAnimation(float time){
	D3DXVECTOR3 position =	this->getPosition();
	D3DXVECTOR3 rotation = this->getRotation();
	time *= 20;
	if (pulled == false){
		moveDistance += time;
		position.z += time * cos(rotation.y);
		position.x += time * sin(rotation.y);
		if (moveDistance > CYLINDER_MOVEDISTANCE){
			pulled = true;
		}
	}
	else{
		moveDistance -= time;
		position.z -= time * cos(rotation.y);
		position.x -= time * sin(rotation.y);
		if (moveDistance < 0){
			pulled = false;
			animationInit = false;
			moveDistance = 0;
			HitCallback();
			playing = false;
		}
	}
	this->setPosition(position);

}
void CCue::setRotationRelative(const D3DXVECTOR3& position){
	D3DXVECTOR3 thisPosition = this->getPosition();
	D3DXVECTOR3 relative = position;
//	D3DXVec3Normalize(&thisPosition, &thisPosition);
//	D3DXVec3Normalize(&relative, &relative);
	float dot = D3DXVec3Dot(&thisPosition, &relative);
	float cross = thisPosition.x * relative.y - thisPosition.y * relative.x;
	float angle = (atan2(cross, dot) * 180 / PI) + 180;
	//this->setRotation(0, 90, 0);
	this->setRotation(0,-angle, 0);

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