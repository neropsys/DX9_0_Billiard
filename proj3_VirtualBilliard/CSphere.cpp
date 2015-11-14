#include "CSphere.h"
#include <d3dx9math.h>
#include <d3d9.h>
#include "ConstVariable.h"
#define CENTERDESTINATION(x1, x2, y1, y2) (sqrtf((pow(x1 - x2, 2)) + (pow(y1 - y2, 2))))


CSphere::CSphere() {

	m_radius = 0;
	m_pMesh = NULL;

	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
}

CSphere::~CSphere(){}

bool CSphere::create(IDirect3DDevice9* pDevice, D3DCOLOR color){
	if (CObject::create(pDevice, Shape::SPHERE) == false) return false;
	LPD3DXMESH newMesh = convertMesh(pDevice, m_pMesh);
	if (newMesh == nullptr) return false;
	if (color == d3d::RED){
		textureFile = SPHERE_RED;
	}
	else if (color == d3d::YELLOW){
		textureFile = SPHERE_YELLOW;
	}
	else if (color == d3d::WHITE){
		textureFile = SPHERE_WHITE;
	}
	else{
		textureFile = SPHERE_BLUE;
	}
	effectFile = SPHERE_EFFECT;
	m_texture = LoadTexture(pDevice, textureFile);
	m_effect = LoadShader(pDevice, effectFile);


	if (m_texture == nullptr || m_effect == nullptr)
		return false;

	m_pMesh->Release();
	m_pMesh = newMesh;

	return true;
}


void CSphere::tempdraw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView,
	const D3DXVECTOR4& mLightPos){
	if (NULL == pDevice)
		return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	D3DXMATRIX proj;
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	m_effect->SetMatrix("gLocalMatrix", &m_mLocal);
	m_effect->SetMatrix("gWorldMatrix", &mWorld);
	m_effect->SetMatrix("gViewMatrix", &mView);
	m_effect->SetVector("gWorldLightPosition", &mLightPos);
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



bool CSphere::hasIntersected(CSphere& ball)
{
	const D3DXVECTOR3 ret = ball.getCenter() - this->getCenter();
	return D3DXVec3Length(&ret) < 2 * this->getRadius();
	//프레임 위치 이용한 계산.
}

//calculate absolute value of a given vector( = |u| )
float abs_vector(float x, float y){
	return sqrtf(pow(x, 2) + pow(y, 2));
}

void CSphere::hitBy(CSphere& ball){
	if (hasIntersected(ball))
	{
		D3DXVECTOR3 avec, a1, a2;

		float dest;	//각각 벡터 v1과 v2의 x, z성분
 		const D3DXVECTOR3 centVec = ball.getCenter() - this->getCenter();
		dest = D3DXVec3Length(&centVec);
		avec = (ball.getCenter() - this->getCenter()) / dest;

		
		a1 = (D3DXVec3Dot(&avec, &this->velocity) * avec);
		a2 = (D3DXVec3Dot(&avec, &ball.velocity) * avec);

		this->setPower(this->getVelocity() - a1 + a2);
		ball.setPower(ball.getVelocity() - a2 + a1);
		
		//공을 떼어놓는다
		if (this->getPower() > ball.getPower()){
			this->moveCenter((2 * this->getRadius() - dest) * avec * 2);
		}
		else{
			ball.moveCenter((2 * this->getRadius() - dest) * avec * -2);
		}
		
		
		
	}
	//maybe spin will be written here.
}

void CSphere::ballUpdate(float timeDiff)
{
	const float TIME_SCALE = 3.3;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(velocity.x);
	double vz = abs(velocity.z);

	if (vx > 0.01 || vz > 0.01)
	{
		float tX = cord.x + TIME_SCALE*timeDiff*velocity.x;
		float tZ = cord.z + TIME_SCALE*timeDiff*velocity.z;

		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(velocity.x * rate, velocity.z * rate);
}

void CSphere::setCenter(float x, float y, float z) {

	setPosition(x, y, z);

}
void CSphere::moveCenter(D3DXVECTOR3 vel){
	this->getCenter() + vel;
}
LPD3DXMESH CSphere::convertMesh(IDirect3DDevice9* pDevice, LPD3DXMESH& mesh){
	LPD3DXMESH newMesh = nullptr;

	mesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);

	VERTEX* pVerts;
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			D3DXVECTOR3 v = pVerts->pos - getCenter();
			D3DXVec3Normalize(&v, &v);
			pVerts->tu = asin(v.x) / D3DX_PI + .5f;
			pVerts->tv = asin(v.y) / D3DX_PI + .5f;
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
	}
	return newMesh;
}
