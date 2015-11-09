#include "CSphere.h"
#include <d3dx9math.h>
#include <d3d9.h>
#define CENTERDESTINATION(x1, x2, y1, y2) (sqrtf((pow(x1 - x2, 2)) + (pow(y1 - y2, 2))))


CSphere::CSphere() {

	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_pMesh = NULL;

	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
}
CSphere::~CSphere(){}
bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color){
	if (color == d3d::RED){
		m_texture = LoadTexture(pDevice, SPHERE_RED);
	}
	else if (color == d3d::YELLOW){
		m_texture = LoadTexture(pDevice, SPHERE_YELLOW);
	}
	else if (color == d3d::WHITE){
		m_texture = LoadTexture(pDevice, SPHERE_WHITE);
	}
	else{
		m_texture = LoadTexture(pDevice, SPHERE_BLUE);
	}
	//m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
	if (!m_texture)
		return false;
		
	m_effect = LoadShader(pDevice, SPHERE_EFFECT);
	if (!m_effect)
		return false;
	m_pMesh = createMesh(pDevice, getRadius(), 50, 50);

	return true;
}
void CSphere::destroy() {

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
void CSphere::draw(IDirect3DDevice9* pDevice,
	const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView)
{
	if (NULL == pDevice)
		return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	D3DXMATRIX proj;
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	m_effect->SetMatrix("gLocalMatrix", &m_mLocal);
	m_effect->SetMatrix("gWorldMatrix", &mWorld);
	m_effect->SetMatrix("gViewMatrix", &mView);
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
			this->moveCenter((2 * this->getRadius() - dest) * -1 * avec);
		}
		else{
			ball.moveCenter((2 * this->getRadius() - dest) * 1 * avec);
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

		//correction of position of ball
		/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if (tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
		else if (tX <= (-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
		else if (tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
		else if (tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;
		*/
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
	D3DXVECTOR3 temp = this->getCenter() + vel;
	this->setCenter(temp.x, temp.y, temp.z);
}
void CSphere::setPosition(float x, float y, float z) {
	D3DXMATRIX m;
	m_x = x;	m_y = y;	m_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
	LPD3DXBUFFER pError = NULL;
	LPD3DXEFFECT ret = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;

#endif
	D3DXCreateEffectFromFile(pDevice, fileName, 0, 0, dwShaderFlags, 0, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack) {
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}

	/*
	D3DXCompileShaderFromFile(
		SPHERE_VS_NAME,
		0,
		0,
		"main",
		"vs_3_0",
		dwShaderFlags,
		&ret,
		&pError,
		&m_constTable);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}
	if (FAILED(pDevice->CreateVertexShader((DWORD*)ret->GetBufferPointer(), &m_shader))){
		OutputDebugString("Failed to create vertex shader");
		return NULL;
	}
	
	//D3DXCompileShaderFromFile(fileName, 0, 0, "main", "vs_3_0", dwShaderFlags, &m_effect, &pError, NULL);
	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}*/

	return ret;

}

LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName) {
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))) {
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}
LPD3DXMESH CSphere::createMesh(IDirect3DDevice9* pDevice, float rad, UINT slices, UINT stacks){
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDevice, rad, slices, stacks, &mesh, NULL))){
		return NULL;
	}

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, sizeof(D3DXVECTOR3) * 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	//IDirect3DVertexDeclaration9* ppDecl;
	//DWORD numFaces = mesh->GetNumFaces();
	//DWORD numVertices = mesh->GetNumVertices();
	//DWORD options = mesh->GetOptions();

	LPD3DXMESH newMesh;
	mesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);


	//if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pDevice, &newMesh))){
	//	return mesh;
	//}
	//if (FAILED(D3DXCreateMesh(numFaces, numVertices, options, decl, pDevice, &newMesh))){
	//	return mesh;
	//}
	//if (FAILED(pDevice->CreateVertexDeclaration(decl, &ppDecl))){
	//	return mesh;
//	}
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

	/*
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			pVerts->tu = asin(pVerts->norm.x) / D3DX_PI + .5f;
			pVerts->tv = asin(pVerts->norm.y) / D3DX_PI + .5f;
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
	}*/
	mesh->Release();
	return newMesh;

}
